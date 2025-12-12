#include "graphics.h"

void drawControlUI(const controlUI &_ui, const int &_order)
{
    //draw right text dynamically
    const char *rightText = TextFormat("%.2f", _ui.refVal);

    //draw slider
    float originalSliderWidth = GetScreenWidth() - 2 * _ui.sideSpace;
    GuiSliderBar(Rectangle{_ui.sideSpace, _order * _ui.uiPadding + (_order - 1) * _ui.fontSize, originalSliderWidth * _ui.proportion, (float)_ui.fontSize}, "", "", &_ui.refVal, _ui.min, _ui.max);

    //draw left text
    DrawText(_ui.leftText, (int)_ui.sideSpace - MeasureText(_ui.leftText, _ui.fontSize) - (int)_ui.uiPadding, _order * (int)_ui.uiPadding + (_order - 1) * _ui.fontSize, _ui.fontSize, WHITE);

    //draw right text
    DrawText(rightText, GetScreenWidth() - (int)(_ui.sideSpace + originalSliderWidth * (1.0f - _ui.proportion)) + (int)_ui.uiPadding, _order * (int)_ui.uiPadding + (_order - 1) * _ui.fontSize, _ui.fontSize, WHITE);
}
void draw(const std::vector<controlUI> &_sliders,
          const std::vector<std::function<void()>> &_uis,
          const std::vector<PhysicsBody *> &_objs)
{
    BeginDrawing();
    ClearBackground(BLACK);

    //draw sliders and texts
    for (int i = 0; i < static_cast<int>(_sliders.size()); i++)
        drawControlUI(_sliders[i], i + 1);
    //draw other ui elements
    for (auto &_ui : _uis)
        _ui();

    //draw helper visual tools
    DrawCircleV({x, y}, 5.0f, RED);
    DrawLineV({x, y}, {x + speedX * drawLineLengthFactor, y + speedY * drawLineLengthFactor}, RED);
    DrawLineV({x, y}, {x, y + g * drawLineLengthFactor}, GREEN);

    //draw physic bodies
    for (auto *obj : _objs)
        obj->draw();

    //draw on top of everything so codes appear last
    DrawText("Wen Wen 101539831", 10, InitialHeight - 30, 20, WHITE);
    const char *objCount = TextFormat("%d", PhysicsShape::count);
    DrawText(objCount, InitialWidth - 10 - MeasureText(objCount, 30), InitialHeight - 40, 30, WHITE);
    EndDrawing();
}
const char *getHalfspacesForDropdown(const std::vector<Halfspace *> &_hss)
{
    dropdownItemsStr.clear();
    for (size_t i = 0; i < _hss.size(); i++)
    {
        dropdownItemsStr += ("Halfspace " + std::to_string(i));
        if (i < _hss.size() - 1)
            dropdownItemsStr += ";";
    }
    return dropdownItemsStr.c_str();
}