#pragma once
#include "physics.h"

struct controlUI
{
    controlUI(
        float _padding, float _side, int _fontSize,
        float &_val, float _min, float _max, const char *_left, float _prop = 1.0f)
        : uiPadding(_padding), sideSpace(_side), fontSize(_fontSize),
          refVal(_val), min(_min), max(_max), leftText(_left), proportion(_prop) {}
    float uiPadding, sideSpace;
    int fontSize;
    float &refVal, min, max;
    const char *leftText;
    float proportion;
};
void draw(const std::vector<controlUI> &_sliders,
          const std::vector<std::function<void()>> &_uis,
          const std::vector<PhysicsBody *> &_objs);
const char *getHalfspacesForDropdown(const std::vector<Halfspace *> &_hss);