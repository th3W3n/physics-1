#pragma once
#include "game.h"

extern float x, xMin, xMax;
extern float y, yMin, yMax;
extern float angle, angleMin, angleMax;
extern float speed, speedMin, speedMax;
extern float speedX, speedY;
extern float g, gMin, gMax;

struct PhysicsBody
{
    Vector2 position, velocity;
    float mass, drag;
    Color color;
    float radius;
    PhysicsBody(
        Vector2 _pos = {0.0f, 0.0f}, Vector2 _vel = {0.0f, 0.0f}, float _m = 1.0f, float _d = 0.0f);
};
class PhysicsSimulation
{
  public:
    std::vector<PhysicsBody> objs;

  public:
    PhysicsSimulation(int _fps = 50);
    void tick();
    int getFPS() const;

  private:
    int m_fps; //frames/second
    Vector2 m_g;
    float m_deltaTime, m_time;

  private:
    void destroyOutOfBounds(PhysicsBody &_objToDestroy, PhysicsBody &_objToSwitchIndex);
};