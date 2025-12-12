#define RAYGUI_IMPLEMENTATION
#include "graphics.h"

bool isDebugging = false;

int main()
{
    InitWindow(InitialWidth, InitialHeight, "GAME2005 - Wen Wen 101539831");

    //random seed
    srand((unsigned int)time(NULL));

    PhysicsSimulation ps;
    SetTargetFPS(ps.getFPS());

    std::vector<controlUI> sliders;
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, mass, massMin, massMax, "Mass");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, mu, muMin, muMax, "Friction Mu");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, x, xMin, xMax, "Launch Pos X");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, y, yMin, yMax, "Launch Pos Y");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, angle, angleMin, angleMax, "Launch Angle");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, speed, speedMin, speedMax, "Launch Speed");
    sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, g, gMin, gMax, "Gravity");

    std::vector<std::function<void()>> uis;
    GuiSetStyle(DEFAULT, TEXT_SIZE, uiFontSize);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLUE));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    uis.emplace_back([]() {
        if (GuiButton(Rectangle{1010, 220, 150, 50}, "New Halfspace"))
            createHalfspace = true;
    });
    uis.emplace_back([]() {
        if (GuiButton(Rectangle{1010, 280, 150, 50}, "Circle / AABB"))
            isLaunchingAABB = !isLaunchingAABB;
    });

    while (!WindowShouldClose())
    {
        conditionalBreakpointDebug(); //TEST debug utility

        if (createHalfspace && sliders.size() == 7)
        {
            sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, halfspaceY, halfspaceYMin, halfspaceYMax, "Halfspace Y", 0.7f);
            sliders.emplace_back(uiPaddingSize, sliderIndent, uiFontSize, halfspaceRot, halfspaceRotMin, halfspaceRotMax, "Halfspace Rot", 0.7f);
            uis.emplace_back([&ps]() {
                if (GuiDropdownBox(Rectangle{850, 220, 150, 50}, getHalfspacesForDropdown(ps.hss), &dropdownActive, dropdownEditMode))
                {
                    dropdownEditMode = !dropdownEditMode;
                    halfspaceY = ps.hss[dropdownActive]->position.y;
                    halfspaceRot = ps.hss[dropdownActive]->recordedSliderAngleDeg;
                }
            });
        }
        ps.tick();
        draw(sliders, uis, ps.objs);
    }
    CloseWindow();

    return 0;
}