#include "physics.h"
#include <limits>

float x = sliderIndent, xMin = sliderIndent, xMax = (float)InitialWidth - sliderIndent;
float y = (float)InitialHeight - sliderIndent, yMin = sliderIndent * 1.5f, yMax = (float)InitialHeight - sliderIndent;
float angle = 0.0f, angleMin = 0.0f, angleMax = 360.0f;
float speed = 0.0f, speedMin = 0.0f, speedMax = 2000.0f;
float speedX, speedY;
float g = 0.0f, gMin = -1000.0f, gMax = 1000.0f;
float halfspaceY = (float)InitialHeight - 100.0f, halfspaceYMin = 0.0f, halfspaceYMax = 1000.0f;
float halfspaceRot = 0.0f, halfspaceRotMin = 0.0f, halfspaceRotMax = 360.0f;
bool createHalfspace = false;
int dropdownActive = 0;
bool dropdownEditMode = false;
std::string dropdownItemsStr = "";
float mass = 10.0f, massMin = 1.0f, massMax = 100.0f;
float mu = 0.5f, muMin = 0.0f, muMax = 1.0f;

int PhysicsShape::count = 0;

PhysicsBody::PhysicsBody(Vector2 _pos) : position(_pos)
{
    unsigned char red = (rand() % 101); //red is reserved for collision so make the initial red less
    unsigned char green = (rand() % 256);
    unsigned char blue = (rand() % 256);
    initColor = color = {red, green, blue, 255};
}
PhysicsShape::PhysicsShape(Vector2 _pos, Vector2 _vel, float _m, float _mu) : PhysicsBody(_pos), velocity(_vel), mass(_m), mu(_mu)
{
    count++;
    fg = fn = ff = {0.0f, 0.0f};
}
PhysicsShape::~PhysicsShape()
{
    count--;
}
Circle::Circle(Vector2 _pos, Vector2 _vel, float _m, float _mu) : PhysicsShape(_pos, _vel, _m, _mu)
{
    type = PHTypes::CIRCLE;
    radius = 10.0f + (rand() % 31); //10-40
}
void Circle::draw()
{
    DrawCircleV(position, radius, color);
    DrawLineV(position, position + fg * drawLineLengthFactorForce, PURPLE);
    DrawLineV(position, position + fn * drawLineLengthFactorForce, GREEN);
    DrawLineV(position, position + ff * drawLineLengthFactorForce, ORANGE);
    DrawLineV(position, position + velocity * drawLineLengthFactor, RED);
}
Halfspace::Halfspace(Vector2 _pos) : PhysicsBody(_pos)
{
    type = PHTypes::HALFSPACE;
    recordedSliderAngleDeg = rotateAngleRad = 0.0f;
    normal = {0.0f, -1.0f}; //pointing upward by default
}
void Halfspace::draw()
{
    DrawCircleV(position, 5.0f, color);
    DrawLineV(position, position + normal * 50, color);
    Vector2 surface = Vector2Rotate({1.0f, 0.0f}, rotateAngleRad) * 3000.0f;
    DrawLineV(position - surface, position + surface, color);
}

PhysicsSimulation::PhysicsSimulation(int _fps) : m_g({0.0f, g})
{
    m_fps = (_fps < 1) ? 1 : _fps;
    m_deltaTime = 1.0f / m_fps;
    m_time = 0.0f;
    objs.reserve(100); //reserve for faster performance
    hss.reserve(10);
}
PhysicsSimulation::~PhysicsSimulation()
{
    for (auto *obj : objs)
        delete obj;
}
void PhysicsSimulation::tick()
{
    m_time += m_deltaTime;
    //-------------------------halfspaces-------------------------
    if (createHalfspace) //pass slider params at creation of new halfspace
    {
        createHalfspace = false;
        Halfspace *hs = new Halfspace();
        hs->position.y = halfspaceY;
        hs->recordedSliderAngleDeg = halfspaceRot;
        hs->rotateAngleRad = -halfspaceRot * DEG2RAD;
        hs->normal = Vector2Rotate({0.0f, -1.0f}, hs->rotateAngleRad);
        objs.push_back(hs);
        hss.push_back(hs);
    }
    if (!hss.empty()) //pass slider params when selected as currently active halfspace
    {
        hss[dropdownActive]->position.y = halfspaceY;
        hss[dropdownActive]->recordedSliderAngleDeg = halfspaceRot;
        hss[dropdownActive]->rotateAngleRad = -halfspaceRot * DEG2RAD;
        hss[dropdownActive]->normal = Vector2Rotate({0.0f, -1.0f}, hss[dropdownActive]->rotateAngleRad);
    }
    //-------------------------other physics shapes-------------------------
    if (m_g.y != g) // Only update when changed
        m_g.y = g;
    static float lastAngle = 0.0f, lastSpeed = 0.0f;
    if (lastAngle != angle || lastSpeed != speed) // Only update when changed
    {
        float angleInRadiant = -angle * DEG2RAD;
        speedX = cosf(angleInRadiant) * speed;
        speedY = sinf(angleInRadiant) * speed;
        lastAngle = angle;
        lastSpeed = speed;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        Circle *circle = new Circle(Vector2{x, y}, Vector2{speedX, speedY}, mass, mu);
        circle->fg = m_g * circle->mass;
        objs.push_back(circle);
    }
    testFunc(); //TEST codes
    //-------------------------simulation pipeline-------------------------
    updateState();
    resetState();
    handleCollisions();
    destroyPhysicsShapeOutOfBounds();
}
int PhysicsSimulation::getFPS() const { return m_fps; }

void PhysicsSimulation::updateState()
{
    for (auto &obj : objs)
    {
        if (obj->type == PHTypes::PH_SHAPES)
        {
            PhysicsShape *shape = (PhysicsShape *)obj;
            Vector2 acceleration = (shape->fg + shape->fn + shape->ff) / shape->mass;
            //velocity and position are not reset, they continue from last frame
            shape->velocity += acceleration * m_deltaTime;
            shape->position += shape->velocity * m_deltaTime;
        }
    }
}
void PhysicsSimulation::resetState()
{
    for (auto &obj : objs)
    {
        obj->color = obj->initColor; //reset color per frame
        if (obj->type == PHTypes::PH_SHAPES)
        {
            PhysicsShape *shape = (PhysicsShape *)obj;
            //reset fn/ff per frame (fg stays constant)
            shape->fg = shape->fn = shape->ff = {0.0f, 0.0f};
            shape->fg = m_g * shape->mass;
        }
    }
}
void PhysicsSimulation::handleCollisions()
{
    size_t size = objs.size();
    for (size_t i = 0; i < size; i++)
    {
        PhysicsBody *a = objs[i];
        for (size_t j = i + 1; j < size; j++)
        {
            PhysicsBody *b = objs[j];
            if (a->type == PHTypes::CIRCLE && b->type == PHTypes::CIRCLE)
                handleCollision_CircleCircle((Circle *)a, (Circle *)b);
            else if (a->type == PHTypes::CIRCLE && b->type == PHTypes::HALFSPACE)
                handleCollision_CircleHalfspace((Circle *)a, (Halfspace *)b);
            else if (a->type == PHTypes::HALFSPACE && b->type == PHTypes::CIRCLE)
                handleCollision_CircleHalfspace((Circle *)b, (Halfspace *)a);
        }
    }
}
void PhysicsSimulation::destroyPhysicsShapeOutOfBounds()
{
    //destroy the ball when out of bounds
    for (int i = static_cast<int>(objs.size()) - 1; i >= 0; i--)
    {
        PhysicsBody *obj = objs[i];
        if (obj->type == PHTypes::PH_SHAPES)
        {
            if (obj->position.x < 0.0f || obj->position.x > (float)InitialWidth ||
                obj->position.y < 0.0f || obj->position.y > (float)InitialHeight)
            {
                delete obj;
                objs[i] = objs.back();
                objs.pop_back();
            }
        }
    }
}
void PhysicsSimulation::handleCollision_CircleCircle(Circle *_a, Circle *_b)
{
    //using squared comparison is more performant
    Vector2 displacementAtoB = _b->position - _a->position;
    float distanceSquared = Vector2LengthSqr(displacementAtoB);
    float sumOfRadii = _a->radius + _b->radius;
    if (sumOfRadii * sumOfRadii > distanceSquared)
    {
        float distance = Vector2Length(displacementAtoB);
        float overlap = sumOfRadii - distance;

        Vector2 dirAtoB;
        //if 2 circles are too close (e.g. both created at the same spot)
        if (distance <= std::numeric_limits<float>::epsilon())
        {
            float randomAngle = (rand() % 360) * DEG2RAD; //0-359 degrees in radians
            dirAtoB = {cosf(randomAngle), sinf(randomAngle)};
        }
        else
            //since distance cannot be 0, so it's also necessary to add the if above
            dirAtoB = displacementAtoB / distance;
        Vector2 mtv = dirAtoB * overlap; //minimum translation vector
        _a->position -= mtv * 0.5f;
        _b->position += mtv * 0.5f;
        _a->color = _b->color = RED;
    }
}
void PhysicsSimulation::handleCollision_CircleHalfspace(Circle *_cir, Halfspace *_hs)
{
    Vector2 displacementToCircle = _cir->position - _hs->position;
    float dotProduct = Vector2DotProduct(displacementToCircle, _hs->normal);
    float overlap = _cir->radius - dotProduct;
    if (overlap >= 0)
    {
        //correct penetration
        Vector2 mtv = _hs->normal * overlap;
        _cir->position += mtv;
        _cir->color = _hs->color = RED;

        //correct forces
        Vector2 fgPerp = _hs->normal * Vector2DotProduct(_cir->fg, _hs->normal);
        _cir->fn = fgPerp * -1;
        Vector2 fgPara = _cir->fg - fgPerp;
        float frictionMagnitudeMax = _cir->mu * Vector2Length(_cir->fn);
        if (Vector2Length(fgPara) <= frictionMagnitudeMax)
            _cir->ff = fgPara * -1;
        else
            _cir->ff = Vector2Normalize(fgPara * -1) * frictionMagnitudeMax;
    }
}
void PhysicsSimulation::testFunc()
{
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_E) || IsKeyPressed(KEY_R))
    {
        Circle *circle;
        if (IsKeyPressed(KEY_Q))
        {
            circle = new Circle(Vector2{x, y}, Vector2{speedX, speedY}, 2.0f, 0.1f);
            circle->initColor = circle->color = RED;
        }
        else if (IsKeyPressed(KEY_W))
        {
            circle = new Circle(Vector2{x, y}, Vector2{speedX, speedY}, 2.0f, 0.8f);
            circle->initColor = circle->color = GREEN;
        }
        else if (IsKeyPressed(KEY_E))
        {
            circle = new Circle(Vector2{x, y}, Vector2{speedX, speedY}, 8.0f, 0.1f);
            circle->initColor = circle->color = BLUE;
        }
        else
        {
            circle = new Circle(Vector2{x, y}, Vector2{speedX, speedY}, 8.0f, 0.8f);
            circle->initColor = circle->color = YELLOW;
        }
        circle->fg = m_g * circle->mass;
        objs.push_back(circle);
    }
}