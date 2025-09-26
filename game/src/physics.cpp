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
    unsigned char red = (rand() % 256);
    unsigned char green = (rand() % 256);
    unsigned char blue = (rand() % 256);
    color = {red, green, blue, 255};
    radius = 10.0f + (rand() % 31); //10-40
}
PhysicsSimulation::PhysicsSimulation(int _fps) : m_g({0.0f, g})
{
    m_fps = (_fps < 1) ? 1 : _fps;
    m_deltaTime = 1.0f / m_fps;
    m_time = 0.0f;
    objs.reserve(100); //reserve for faster performance
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
        objs.emplace_back(Vector2{x, y}, Vector2{speedX, speedY});
    }
    for (int i = static_cast<int>(objs.size()) - 1; i >= 0; i--)
    {
        auto &obj = objs[i];
        obj.velocity += m_g * m_deltaTime;
        obj.position += obj.velocity * m_deltaTime;
        destroyOutOfBounds(objs[i], objs.back());
    }
}
//getters and setters
int PhysicsSimulation::getFPS() const { return m_fps; }

void PhysicsSimulation::destroyOutOfBounds(PhysicsBody &_objToDestroy, PhysicsBody &_objToSwitchIndex)
{
    if (_objToDestroy.position.x < 0.0f || _objToDestroy.position.x > (float)InitialWidth ||
        _objToDestroy.position.y < 0.0f || _objToDestroy.position.y > (float)InitialHeight)
    {
        //objs.erase(objs.begin() + i);
        std::swap(_objToDestroy, _objToSwitchIndex);
        objs.pop_back();
    }
}
