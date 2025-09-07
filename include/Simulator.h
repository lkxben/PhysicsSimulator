#pragma once
#include <vector>
#include "Particle.h"
#include "Entity.h"
#include "Renderer.h"
#include "Obstacle.h"
#include "Forcefield.h"
#include "EventManager.h"

class Simulator {
protected:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Forcefield>> forcefields;
    double width, height;
    int cols, rows;
    size_t avgPerCell;
    double cellSize;
    std::vector<std::vector<int>> grid;

public:
    Simulator(std::vector<std::unique_ptr<Obstacle>>& obstacles_, std::vector<std::unique_ptr<Particle>>& particles_, std::vector<std::unique_ptr<Forcefield>>& forcefields_, double w, double h);

    virtual void update(double dt);

    void run(Renderer& renderer, EventManager& events);

    virtual ~Simulator() = default;
};
