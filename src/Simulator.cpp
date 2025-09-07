#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <iostream>
#include "Simulator.h"
#include "Entity.h"
#include "Particle.h"
#include "CollisionChecker.h"

Simulator::Simulator(std::vector<std::unique_ptr<Obstacle>>& obstacles_, std::vector<std::unique_ptr<Particle>>& particles_, std::vector<std::unique_ptr<Forcefield>>& forcefields_, double w, double h)
    : width(w), height(h)
{
    for (auto& p : particles_) {
        particles.push_back(std::move(p));
    }

    for (auto& o : obstacles_) {
        obstacles.push_back(std::move(o));
    }

    for (auto& f : forcefields_) {
        forcefields.push_back(std::move(f));
    }

    double maxRadius = 0.0;
    if (!particles.empty()) {
        maxRadius = std::max_element(
            particles.begin(),
            particles.end(),
            [](const std::unique_ptr<Particle>& a, const std::unique_ptr<Particle>& b) {
                return a->radius < b->radius;
            }
        )->get()->radius;
    }

    cellSize = 2 * maxRadius;
    cols = static_cast<int>(width / cellSize) + 1;
    rows = static_cast<int>(height / cellSize) + 1;
    avgPerCell = std::max<size_t>(1, particles.size() / (cols * rows));
    grid.resize(cols * rows);
}

void Simulator::update(double dt) {
    for (auto& cell : grid) {
        cell.clear();
        cell.reserve(avgPerCell);
    }

    // Insert particles into spatial grid
    for (size_t i = 0; i < particles.size(); ++i) {
        Particle* p = particles[i].get();
        int row = static_cast<int>(p->y / cellSize);
        int col = static_cast<int>(p->x / cellSize);
        row = std::clamp(row, 0, rows - 1);
        col = std::clamp(col, 0, cols - 1);
        grid[row * cols + col].push_back(static_cast<int>(i));
    }

    // Move particles and resolve collisions
    for (size_t i = 0; i < particles.size(); ++i) {
        Particle* p1 = particles[i].get();
        // std::cout << "x: " << p1->x << " , y: " << p1->y;
        p1->move(dt);

        // Boundary collisions
        if (p1->x - p1->radius < 0) { p1->vx = -p1->vx; p1->x = p1->radius; }
        else if (p1->x + p1->radius > width) { p1->vx = -p1->vx; p1->x = width - p1->radius; }

        if (p1->y - p1->radius < 0) { p1->vy = -p1->vy; p1->y = p1->radius; }
        else if (p1->y + p1->radius > height) { p1->vy = -p1->vy; p1->y = height - p1->radius; }

        int row = static_cast<int>(p1->y / cellSize);
        int col = static_cast<int>(p1->x / cellSize);
        row = std::clamp(row, 0, rows - 1);
        col = std::clamp(col, 0, cols - 1);
        int i_int = static_cast<int>(i);

        // Check collisions with nearby particles
        for (int r = std::max(0, row - 1); r <= std::min(rows - 1, row + 1); ++r) {
            // Apply forcefield
            for (auto& field : forcefields) {
                field->apply(*particles[i], dt);
            }

            for (int c = std::max(0, col - 1); c <= std::min(cols - 1, col + 1); ++c) {
                for (int j : grid[r * cols + c]) {
                    if (j <= i_int) continue;
                    CollisionChecker::check(*p1, *particles[j], dt);
                }
            }

            // Collision with obstacles
            for (auto& obs : obstacles) {
                CollisionChecker::check(*particles[i], *obs, dt);
            }
        }
    }
}

void Simulator::run(Renderer& renderer, EventManager& events) {
    sf::Clock clock;

    while (renderer.isRunning()) {
        events.pollEvents();
        double dt = clock.restart().asSeconds();
        update(dt);
        renderer.draw(particles, obstacles, forcefields);
    }
}
