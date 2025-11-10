#pragma once
#include "game.h"

extern float x, xMin, xMax;
extern float y, yMin, yMax;
extern float angle, angleMin, angleMax;
extern float speed, speedMin, speedMax;
extern float speedX, speedY;
extern float g, gMin, gMax;
extern float halfspaceY, halfspaceYMin, halfspaceYMax;
extern float halfspaceRot, halfspaceRotMin, halfspaceRotMax;
extern bool createHalfspace;
extern int dropdownActive;
extern bool dropdownEditMode;
extern std::string dropdownItemsStr;

enum class PHTypes : uint8_t
{
    //use flags enum class
    NONE = 0,
    CIRCLE = 1 << 0,
    HALFSPACE = 1 << 1,
    PH_SHAPES = CIRCLE
};
struct PhysicsBody
{
    Vector2 position;
    Color initColor, color;
    PHTypes type;
    virtual void draw() = 0;
    virtual ~PhysicsBody() = default;

  protected:
    PhysicsBody(Vector2 _pos);
};
struct PhysicsShape : PhysicsBody
{
    static int count;
    Vector2 velocity;
    float mass, drag;
    ~PhysicsShape();

  protected:
    PhysicsShape(Vector2 _pos, Vector2 _vel, float _m, float _d);
};
struct Circle : PhysicsShape
{
    float radius;
    Circle(
        Vector2 _pos = {0.0f, 0.0f}, Vector2 _vel = {0.0f, 0.0f}, float _m = 1.0f, float _d = 0.0f);
    void draw() override;
};
struct Halfspace : PhysicsBody
{
    float recordedSliderAngleDeg;
    float rotateAngleRad;
    Vector2 normal;
    Halfspace(Vector2 _pos = {InitialWidth / 2.0f, halfspaceY});
    void draw() override;
};

class PhysicsSimulation
{
  public:
    std::vector<PhysicsBody *> objs; //use pointers for elements to prevent object slicing (for derived objects)
    std::vector<Halfspace *> hss;

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
    void handleCollision_CircleCircle(Circle *_a, Circle *_b);
    void handleCollision_CircleHalfspace(Circle *_cir, Halfspace *_hs);
};