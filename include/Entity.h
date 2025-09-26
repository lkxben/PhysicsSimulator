#pragma once
#include <cstddef>
#include "raylib.h"

struct Entity {
    double x, y;
    Color color;

    Entity(double x_, double y_, Color color_ = WHITE)
        : x(x_), y(y_), color(color_) {}

    virtual ~Entity() = default;
    virtual void draw() const = 0;
};
