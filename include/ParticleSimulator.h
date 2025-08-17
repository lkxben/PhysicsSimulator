#pragma once
#include <vector>
#include "Particle.h"

class ParticleSimulator {
    std::vector<Particle>& particles;
    double width, height;
    int cols, rows;
    size_t avgPerCell;
    double cellSize;
    std::vector<std::vector<int>> grid;

public:
    ParticleSimulator(std::vector<Particle>& particles_, double w, double h);

    void update(double dt);

    void resolveCollision(Particle& p1, Particle& p2);
};
