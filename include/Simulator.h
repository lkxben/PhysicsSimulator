#pragma once
#include <vector>
#include "Particle.h"
#include "Entity.h"
#include "Renderer.h"
#include "Obstacle.h"

class Simulator {
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Particle>> particles;
    double width, height;
    int cols, rows;
    size_t avgPerCell;
    double cellSize;
    std::vector<std::vector<int>> grid;

public:
    Simulator(std::vector<std::unique_ptr<Obstacle>>& obstacles_, std::vector<std::unique_ptr<Particle>>& particles_, double w, double h);

    void update(double dt);

    void resolveCollision(Particle& p1, Particle& p2);

    void run(Renderer& renderer);
};
