#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include "Simulator.h"
#include "Entity.h"
#include "Particle.h"
#include "CollisionChecker.h"

Simulator::Simulator(std::vector<std::unique_ptr<Obstacle>>& obstacles_, std::vector<std::unique_ptr<Particle>>& particles_, double w, double h)
    : width(w), height(h)
{
    for (auto& p : particles_) {
        particles.push_back(std::move(p));
    }

    for (auto& o : obstacles_) {
        obstacles.push_back(std::move(o));
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
            for (int c = std::max(0, col - 1); c <= std::min(cols - 1, col + 1); ++c) {
                for (int j : grid[r * cols + c]) {
                    if (j <= i_int) continue;
                    CollisionChecker::check(*p1, *particles[j]);
                }
            }

            // Collision with obstacles
            for (auto& obs : obstacles) {
                CollisionChecker::check(*particles[i], *obs);
            }
        }
    }
}

// void Simulator::resolveCollision(Particle& p1, Particle& p2) {
//     double dx = p2.x - p1.x;
//     double dy = p2.y - p1.y;
//     double dist2 = dx*dx + dy*dy;
//     double radiiSum = p1.radius + p2.radius;

//     if (dist2 < radiiSum * radiiSum) {
//         double dist = std::sqrt(dist2);
//         if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

//         double nx = dx / dist;
//         double ny = dy / dist;

//         double relVel = (p2.vx - p1.vx) * nx + (p2.vy - p1.vy) * ny;
//         if (relVel > 0) return;

//         double e = std::min(p1.elasticity, p2.elasticity);
//         double j = -(1 + e) * relVel / (1/p1.mass + 1/p2.mass);

//         double ix = j * nx;
//         double iy = j * ny;

//         p1.vx -= ix / p1.mass;
//         p1.vy -= iy / p1.mass;
//         p2.vx += ix / p2.mass;
//         p2.vy += iy / p2.mass;

//         double overlap = 0.5 * (radiiSum - dist);
//         p1.x -= overlap * nx;
//         p1.y -= overlap * ny;
//         p2.x += overlap * nx;
//         p2.y += overlap * ny;
//     }
// }

void Simulator::run(Renderer& renderer) {
    sf::Clock clock;

    while (renderer.isRunning()) {
        renderer.pollEvents();
        double dt = clock.restart().asSeconds();
        update(dt);
        renderer.draw(particles, obstacles);
    }
}
