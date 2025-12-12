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
extern float mass, massMin, massMax;
extern float mu, muMin, muMax;
extern bool isLaunchingAABB;

enum class PHTypes : uint8_t
{
    //use flags enum class
    NONE = 0,
    CIRCLE = 1 << 0,
    AABB = 1 << 1,
    HALFSPACE = 1 << 2,
    PH_SHAPES = CIRCLE | AABB
};
inline PHTypes operator&(PHTypes _a, PHTypes _b)
{
    return static_cast<PHTypes>(static_cast<uint8_t>(_a) & static_cast<uint8_t>(_b));
}
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
    bool isStatic = false;
    Vector2 fg, fn, ff; //force by gravity/normal/friction
    Vector2 velocity;
    float mass, mu; //mu: coefficient of friction
    float COR; //bounciness (coefficient of restitution)
    ~PhysicsShape();

  protected:
    PhysicsShape(Vector2 _pos, Vector2 _vel, float _m, float _mu);
};
struct Circle : PhysicsShape
{
    float radius;
    Circle(Vector2 _pos, Vector2 _vel, float _m, float _mu);
    void draw() override;
};
struct AABB : PhysicsShape
{
    Vector2 extents; //(width/2, height/2)
    float xMin, xMax, yMin, yMax;
    AABB(Vector2 _pos, Vector2 _vel, float _m, float _mu);
    void updateMinMax();
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
    //-------------------------getters and setters-------------------------
    int getFPS() const;

  private:
    int m_fps; //frames/second
    Vector2 m_g;
    float m_deltaTime, m_time;

  private:
    //-------------------------simulation pipeline-------------------------
    //predictor-corrector pattern
    void updateState(); //predictor: move as if no collision
    void resetState();
    void handleCollisions(); //corrector: corrects position (mtv) and forces
    void destroyPhysicsShapeOutOfBounds();
    //---------------------------------------------------------------------
    void handleCollision_CircleCircle(Circle *_a, Circle *_b);
    void handleCollision_CircleHalfspace(Circle *_cir, Halfspace *_hs);
    void handleCollision_AABBAABB(AABB *_a, AABB *_b);
    void handleCollision_CircleAABB(Circle *_cir, AABB *_aabb);
    void testFunc(); //TEST codes
};