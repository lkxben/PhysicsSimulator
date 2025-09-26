#pragma once
#include <vector>
#include "System.h"
#include "Particle.h"
#include "Entity.h"

class CollisionSystem : public System {
protected:
    double width, height;
    int cols, rows;
    size_t avgPerCell;
    double cellSize;
    std::vector<std::vector<int>> grid;

public:
    CollisionSystem(const World& world, double w, double h);

    void update(World& world, double dt);

    void check(Entity* a, Entity* b, double dt);

    void check(Particle& p1, Particle& p2, double dt);

    void check(Particle& p, Obstacle& o, double dt);
};
