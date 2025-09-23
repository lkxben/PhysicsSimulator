#pragma once
#include <vector>
#include <memory>
#include "Particle.h"
#include "Obstacle.h"
#include "Forcefield.h"
#include "Constraint.h"

class World {
public:
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    std::vector<std::unique_ptr<Constraint>> constraints;
};