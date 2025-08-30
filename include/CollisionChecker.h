#pragma once
#include <vector>
#include "Particle.h"
#include "Entity.h"
#include "Renderer.h"

namespace CollisionChecker {
    void check(Entity* a, Entity* b, double dt);
    void check(Particle& p1, Particle& p2, double dt);
    void check(Particle& p, Obstacle& o, double dt);
}
