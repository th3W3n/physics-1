#include "graphics.h"

void drawControlUI(const controlUI &_ui)
{
    //draw right text dynamically
    const char *rightText = TextFormat("%.2f", _ui.refVal);

    //draw slider
    GuiSliderBar(Rectangle{_ui.sideSpace, _ui.uiOrder * _ui.uiPadding + (_ui.uiOrder - 1) * _ui.fontSize, GetScreenWidth() - 2 * _ui.sideSpace, (float)_ui.fontSize}, "", "", &_ui.refVal, _ui.min, _ui.max);

    //draw left text
    DrawText(_ui.leftText, (int)_ui.sideSpace - MeasureText(_ui.leftText, _ui.fontSize) - (int)_ui.uiPadding, _ui.uiOrder * (int)_ui.uiPadding + (_ui.uiOrder - 1) * _ui.fontSize, _ui.fontSize, WHITE);

    //draw right text
    DrawText(rightText, GetScreenWidth() - (int)_ui.sideSpace + (int)_ui.uiPadding, _ui.uiOrder * (int)_ui.uiPadding + (_ui.uiOrder - 1) * _ui.fontSize, _ui.fontSize, WHITE);
}
void draw(const std::vector<controlUI> &_uis, const std::vector<PhysicsBody *> &_objs)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Wen Wen 101539831", 10, InitialHeight - 30, 20, LIGHTGRAY);

    //draw sliders and texts
    for (auto &_ui : _uis)
        drawControlUI(_ui);

    //draw helper visual tools
    const char *objCount = TextFormat("%d", (int)_objs.size());
    DrawText(objCount, InitialWidth - 10 - MeasureText(objCount, 30), InitialHeight - 40, 30, BLUE);
    DrawCircleV({x, y}, 5.0f, RED);
    DrawLineV({x, y}, {x + speedX * drawLineLengthFactor, y + speedY * drawLineLengthFactor}, RED);
    DrawLineV({x, y}, {x, y + g * drawLineLengthFactor}, GREEN);

    //draw physic bodies
    for (auto *obj : _objs)
        obj->draw();

    EndDrawing();
}