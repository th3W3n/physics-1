#define RAYGUI_IMPLEMENTATION
#include "graphics.h"

int main()
{
    InitWindow(InitialWidth, InitialHeight, "GAME2005 - Wen Wen 101539831");

    //random seed
    srand((unsigned int)time(NULL));

    PhysicsSimulation ps;
    SetTargetFPS(ps.getFPS());

    std::vector<controlUI> uis;
    uis.emplace_back(1, uiPaddingSize, sliderIndent, uiFontSize, x, xMin, xMax, "Launch Pos X");
    uis.emplace_back(2, uiPaddingSize, sliderIndent, uiFontSize, y, yMin, yMax, "Launch Pos Y");
    uis.emplace_back(3, uiPaddingSize, sliderIndent, uiFontSize, angle, angleMin, angleMax, "Launch Angle");
    uis.emplace_back(4, uiPaddingSize, sliderIndent, uiFontSize, speed, speedMin, speedMax, "Launch Speed");
    uis.emplace_back(5, uiPaddingSize, sliderIndent, uiFontSize, g, gMin, gMax, "Gravity");

    while (!WindowShouldClose())
    {
        ps.tick();
        draw(uis, ps.objs);
    }
    CloseWindow();

    return 0;
}