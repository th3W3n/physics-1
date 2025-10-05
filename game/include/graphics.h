#pragma once
#include "physics.h"

struct controlUI
{
    controlUI(
        int _order, float _padding, float _side, int _fontSize,
        float &_val, float _min, float _max, const char *_left)
        : uiOrder(_order), uiPadding(_padding), sideSpace(_side), fontSize(_fontSize),
          refVal(_val), min(_min), max(_max), leftText(_left) {}
    int uiOrder;
    float uiPadding, sideSpace;
    int fontSize;
    float &refVal, min, max;
    const char *leftText;
};
void drawControlUI(const controlUI &_ui);
void draw(const std::vector<controlUI> &_uis, const std::vector<PhysicsBody *> &_objs);