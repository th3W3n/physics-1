#include "physics.h"

float x = sliderIndent, xMin = sliderIndent, xMax = (float)InitialWidth - sliderIndent;
float y = (float)InitialHeight - sliderIndent, yMin = sliderIndent * 1.5f, yMax = (float)InitialHeight - sliderIndent;
float angle = 0.0f, angleMin = 0.0f, angleMax = 360.0f;
float speed = 0.0f, speedMin = 0.0f, speedMax = 2000.0f;
float speedX, speedY;
float g = 0.0f, gMin = -2000.0f, gMax = 2000.0f;
float halfspaceY = (float)InitialHeight - 100.0f, halfspaceYMin = 250.0f, halfspaceYMax = (float)InitialHeight - 50.0f;
float halfspaceRot = 0.0f, halfspaceRotMin = 0.0f, halfspaceRotMax = 360.0f;

int PhysicsShape::count = 0;

PhysicsBody::PhysicsBody(Vector2 _pos) : position(_pos)
{
    unsigned char red = (rand() % 101); //red is reserved for collision so make the initial red less
    unsigned char green = (rand() % 256);
    unsigned char blue = (rand() % 256);
    initColor = color = {red, green, blue, 255};
}
PhysicsShape::PhysicsShape(Vector2 _pos, Vector2 _vel, float _m, float _d) : PhysicsBody(_pos), velocity(_vel), mass(_m), drag(_d)
{
    count++;
}
PhysicsShape::~PhysicsShape()
{
    count--;
}
Circle::Circle(Vector2 _pos, Vector2 _vel, float _m, float _d) : PhysicsShape(_pos, _vel, _m, _d)
{
    type = PHTypes::CIRCLE;
    radius = 10.0f + (rand() % 31); //10-40
}
void Circle::draw()
{
    DrawCircleV(position, radius, color);
}
Halfspace::Halfspace(Vector2 _pos) : PhysicsBody(_pos)
{
    type = PHTypes::HALFSPACE;
    rotateAngle = 0.0f;
    normal = {0.0f, -1.0f}; //pointing upward by default
}
void Halfspace::draw()
{
    DrawCircleV(position, 5.0f, color);
    DrawLineV(position, position + normal * 50, color);
    Vector2 surface = Vector2Rotate({1.0f, 0.0f}, rotateAngle) * 3000.0f;
    DrawLineV(position - surface, position + surface, color);
}

PhysicsSimulation::PhysicsSimulation(int _fps) : m_g({0.0f, g})
{
    m_fps = (_fps < 1) ? 1 : _fps;
    m_deltaTime = 1.0f / m_fps;
    m_time = 0.0f;
    objs.reserve(100); //reserve for faster performance
    objs.push_back(new Halfspace());
}
PhysicsSimulation::~PhysicsSimulation()
{
    for (auto *obj : objs)
        delete obj;
}
void PhysicsSimulation::tick()
{
    if (m_g.y != g) // Only update when changed
        m_g.y = g;
    m_time += m_deltaTime;
    float angleInRadiant = -angle * DEG2RAD;
    float hsAngleInRadiant = -halfspaceRot * DEG2RAD;
    speedX = cosf(angleInRadiant) * speed;
    speedY = sinf(angleInRadiant) * speed;
    if (IsKeyPressed(KEY_SPACE))
    {
        objs.push_back(new Circle(Vector2{x, y}, Vector2{speedX, speedY}));
    }
    for (int i = static_cast<int>(objs.size()) - 1; i >= 0; i--)
    {
        auto &obj = objs[i];
        obj->color = obj->initColor; //reset color before collision test
        Halfspace *hs = dynamic_cast<Halfspace *>(obj);
        PhysicsShape *shape = dynamic_cast<PhysicsShape *>(obj);

        if (hs)
        {
            hs->position.y = halfspaceY;
            hs->rotateAngle = hsAngleInRadiant;
            hs->normal = Vector2Rotate({0.0f, -1.0f}, hs->rotateAngle);
        }
        else if (shape)
        {
            shape->velocity += m_g * m_deltaTime;
            shape->position += shape->velocity * m_deltaTime;
            destroyOutOfBounds(i);
        }
    }
    checkCollision();
}
void PhysicsSimulation::checkCollision()
{
    size_t size = objs.size();
    for (size_t i = 0; i < size; i++)
    {
        PhysicsBody *a = objs[i];
        for (size_t j = i + 1; j < size; j++)
        {
            PhysicsBody *b = objs[j];
            bool isOverlap = false;
            if (a->type == PHTypes::CIRCLE && b->type == PHTypes::CIRCLE)
                isOverlap = overlapCircleCircle((Circle *)a, (Circle *)b);
            else if (a->type == PHTypes::CIRCLE && b->type == PHTypes::HALFSPACE)
                isOverlap = overlapCircleHalfspace((Circle *)a, (Halfspace *)b);
            else if (a->type == PHTypes::HALFSPACE && b->type == PHTypes::CIRCLE)
                isOverlap = overlapCircleHalfspace((Circle *)b, (Halfspace *)a);
            if (isOverlap)
                a->color = b->color = RED;
        }
    }
}

int PhysicsSimulation::getFPS() const { return m_fps; }

void PhysicsSimulation::destroyOutOfBounds(int _index)
{
    PhysicsBody *obj = objs[_index];
    if (obj->position.x < 0.0f || obj->position.x > (float)InitialWidth ||
        obj->position.y < 0.0f || obj->position.y > (float)InitialHeight)
    {
        delete obj;
        objs[_index] = objs.back();
        objs.pop_back();
    }
}
bool PhysicsSimulation::overlapCircleCircle(Circle *_a, Circle *_b)
{
    //Vector2Length is less efficient, so just compare squared values
    float distanceSquared = Vector2LengthSqr(_b->position - _a->position);
    float sumOfRadii = _a->radius + _b->radius;
    if (sumOfRadii * sumOfRadii > distanceSquared)
        return true;
    else
        return false;
}
bool PhysicsSimulation::overlapCircleHalfspace(Circle *_cir, Halfspace *_hs)
{
    Vector2 displacementToCircle = _cir->position - _hs->position;
    float dotProduct = Vector2DotProduct(displacementToCircle, _hs->normal);
    if (_cir->radius > dotProduct)
        return true;
    else
        return false;
}