#include "CollisionSystem.h"
#include <cmath>

CollisionSystem::CollisionSystem(const World& world, double w, double h)
    : width(w), height(h)
{
    double maxRadius = 0.0;
    if (!world.particles.empty()) {
        maxRadius = std::max_element(
            world.particles.begin(),
            world.particles.end(),
            [](const std::unique_ptr<Particle>& a, const std::unique_ptr<Particle>& b) {
                return a->radius < b->radius;
            }
        )->get()->radius;
    }

    cellSize = 2 * maxRadius;
    cols = static_cast<int>(width / cellSize) + 1;
    rows = static_cast<int>(height / cellSize) + 1;
    avgPerCell = std::max<size_t>(1, world.particles.size() / (cols * rows));
    grid.resize(cols * rows);
}

void CollisionSystem::update(World& world, double dt) {
    for (auto& cell : grid) {
        cell.clear();
        cell.reserve(avgPerCell);
    }

    // Insert particles into spatial grid
    for (size_t i = 0; i < world.particles.size(); ++i) {
        Particle* p = world.particles[i].get();
        int row = static_cast<int>(p->y / cellSize);
        int col = static_cast<int>(p->x / cellSize);
        row = std::clamp(row, 0, rows - 1);
        col = std::clamp(col, 0, cols - 1);
        grid[row * cols + col].push_back(static_cast<int>(i));
    }

    // Move particles and resolve collisions
    for (size_t i = 0; i < world.particles.size(); ++i) {
        Particle* p1 = world.particles[i].get();

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
                    check(*p1, *world.particles[j], dt);
                }
            }

            // Collision with obstacles
            for (auto& obs : world.obstacles) {
                check(*world.particles[i], *obs, dt);
            }
        }
    }
}


void CollisionSystem::check(Entity* a, Entity* b, double dt) {
    if (auto p1 = dynamic_cast<Particle*>(a)) {
        if (auto p2 = dynamic_cast<Particle*>(b)) {
            check(p1, p2, dt);
        } else if (auto o = dynamic_cast<Obstacle*>(b)) {
            check(p1, o, dt);
        }
    }
}

void CollisionSystem::check(Particle& p1, Particle& p2, double dt) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dvx = p2.vx - p1.vx;
    double dvy = p2.vy - p1.vy;

    double radiiSum = p1.radius + p2.radius;

    double relSpeed2 = dvx*dvx + dvy*dvy;
    double threshold = (radiiSum * radiiSum) / (dt * dt);

    bool collided = false;

    if (relSpeed2 > threshold) {
        double a = dvx*dvx + dvy*dvy;
        double b = 2*(dx*dvx + dy*dvy);
        double c = dx*dx + dy*dy - radiiSum*radiiSum;
        double disc = b*b - 4*a*c;
        if (disc >= 0) {
            double t = (-b - std::sqrt(disc)) / (2*a);
            if (t >= 0 && t <= dt) {
                p1.x += p1.vx * t;
                p1.y += p1.vy * t;
                p2.x += p2.vx * t;
                p2.y += p2.vy * t;
                collided = true;
                dt -= t;
            }
        }
    } else {
        double dist2 = dx*dx + dy*dy;
        if (dist2 < radiiSum * radiiSum) {
            collided = true;
        }
    }

    if (!collided) return;

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    double dist2 = dx*dx + dy*dy;
    double dist = std::sqrt(dist2);
    if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

    double nx = dx / dist;
    double ny = dy / dist;

    double relVel = (p2.vx - p1.vx) * nx + (p2.vy - p1.vy) * ny;
    if (relVel > 0) return;

    double e = 0.5 * (p1.elasticity + p2.elasticity);
    double j = -(1 + e) * relVel / (1/p1.mass + 1/p2.mass);

    double ix = j * nx;
    double iy = j * ny;

    p1.vx -= ix / p1.mass;
    p1.vy -= iy / p1.mass;
    p2.vx += ix / p2.mass;
    p2.vy += iy / p2.mass;

    double overlap = 0.5 * ((p1.radius + p2.radius) - dist);
    if (overlap > 0) {
        p1.x -= overlap * nx;
        p1.y -= overlap * ny;
        p2.x += overlap * nx;
        p2.y += overlap * ny;
    }

    if (relSpeed2 > threshold && dt > 0) {
        p1.x += p1.vx * dt;
        p1.y += p1.vy * dt;
        p2.x += p2.vx * dt;
        p2.y += p2.vy * dt;
    }
}

void CollisionSystem::check(Particle& p, Obstacle& o, double dt) {
    o.collide(p, dt);
}
