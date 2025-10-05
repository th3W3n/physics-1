#include "physics.h"

float x = sliderIndent, xMin = sliderIndent, xMax = (float)InitialWidth - sliderIndent;
float y = sliderIndent, yMin = sliderIndent, yMax = (float)InitialHeight - sliderIndent;
float angle = 0.0f, angleMin = 0.0f, angleMax = 360.0f;
float speed = 0.0f, speedMin = 0.0f, speedMax = 2000.0f;
float speedX, speedY;
float g = 0.0f, gMin = -2000.0f, gMax = 2000.0f;

PhysicsBody::PhysicsBody(Vector2 _pos, Vector2 _vel, float _m, float _d)
    : position(_pos), velocity(_vel), mass(_m), drag(_d)
{
    unsigned char red = (rand() % 101); //red is reserved for collision so make the initial red less
    unsigned char green = (rand() % 256);
    unsigned char blue = (rand() % 256);
    initColor = color = {red, green, blue, 255};
}
Circle::Circle(Vector2 _pos, Vector2 _vel, float _m, float _d) : PhysicsBody(_pos, _vel, _m, _d)
{
    radius = 10.0f + (rand() % 31); //10-40
}
void Circle::draw()
{
    DrawCircleV(position, radius, color);
}
PhysicsSimulation::PhysicsSimulation(int _fps) : m_g({0.0f, g})
{
    m_fps = (_fps < 1) ? 1 : _fps;
    m_deltaTime = 1.0f / m_fps;
    m_time = 0.0f;
    objs.reserve(100); //reserve for faster performance
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
    speedX = cosf(angleInRadiant) * speed;
    speedY = sinf(angleInRadiant) * speed;
    if (IsKeyPressed(KEY_SPACE))
    {
        objs.push_back(new Circle(Vector2{x, y}, Vector2{speedX, speedY}));
    }
    for (int i = static_cast<int>(objs.size()) - 1; i >= 0; i--)
    {
        auto &obj = objs[i];
        obj->velocity += m_g * m_deltaTime;
        obj->position += obj->velocity * m_deltaTime;
        obj->color = obj->initColor; //reset color before collision test
        destroyOutOfBounds(i);
    }
    checkCollision();
}
void PhysicsSimulation::checkCollision()
{
    size_t size = objs.size();
    for (size_t i = 0; i < size; i++)
    {
        Circle *a = dynamic_cast<Circle *>(objs[i]);
        if (!a) //skip if not a circle
            continue;
        for (size_t j = i + 1; j < size; j++)
        {
            Circle *b = dynamic_cast<Circle *>(objs[j]);
            if (b && overlapCircleCircle(a, b))
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