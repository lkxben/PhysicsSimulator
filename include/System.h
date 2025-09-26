#pragma once
#include "World.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(World& world, double dt) = 0;
};