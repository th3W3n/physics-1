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
    Color initColor, color;
    virtual void draw() = 0;
    virtual ~PhysicsBody() = default;

  protected:
    PhysicsBody(
        Vector2 _pos, Vector2 _vel, float _m, float _d);
};
struct Circle : PhysicsBody
{
    float radius;
    Circle(
        Vector2 _pos = {0.0f, 0.0f}, Vector2 _vel = {0.0f, 0.0f}, float _m = 1.0f, float _d = 0.0f);
    void draw() override;
};
class PhysicsSimulation
{
  public:
    std::vector<PhysicsBody *> objs; //use pointers for elements to prevent object slicing (for derived objects)

  public:
    PhysicsSimulation(int _fps = 50);
    ~PhysicsSimulation();
    void tick();
    void checkCollision();
    //getters and setters
    int getFPS() const;

  private:
    int m_fps; //frames/second
    Vector2 m_g;
    float m_deltaTime, m_time;

  private:
    void destroyOutOfBounds(int _index);
    bool overlapCircleCircle(Circle *_a, Circle *_b);
};