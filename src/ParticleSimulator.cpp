#include <vector>
#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(std::vector<Particle>& particles_, double w, double h)
    : particles(particles_), width(w), height(h)
{
    double maxRadius = 0.0;
    if (!particles.empty()) {
        maxRadius = std::max_element(
            particles.begin(),
            particles.end(),
            [](const Particle& a, const Particle& b) { return a.radius < b.radius; }
        )->radius;
    }

    cellSize = 2 * maxRadius;
    cols = static_cast<int>(width / cellSize) + 1;
    rows = static_cast<int>(height / cellSize) + 1;
    avgPerCell = std::max<size_t>(1, particles.size() / (cols * rows));
    grid.resize(cols * rows);
}

void ParticleSimulator::update(double dt) {
    for (auto& cell : grid) {
        cell.clear();
        cell.reserve(avgPerCell);
    }
    for (size_t i = 0; i < particles.size(); i++) {
        int row = static_cast<int>(particles[i].y / cellSize);
        int col = static_cast<int>(particles[i].x / cellSize);

        row = std::clamp(row, 0, rows - 1);
        col = std::clamp(col, 0, cols - 1);

        grid[row * cols + col].push_back(static_cast<int>(i));
    }

    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].move(dt);

        if (particles[i].x - particles[i].radius < 0) {
            particles[i].vx = -particles[i].vx;
            particles[i].x = particles[i].radius;
        } else if (particles[i].x + particles[i].radius > width) {
            particles[i].vx = -particles[i].vx;
            particles[i].x = width - particles[i].radius;
        }

        if (particles[i].y - particles[i].radius < 0) {
            particles[i].vy = -particles[i].vy;
            particles[i].y = particles[i].radius;
        } else if (particles[i].y + particles[i].radius > height) {
            particles[i].vy = -particles[i].vy;
            particles[i].y = height - particles[i].radius;
        }

        int row = static_cast<int>(particles[i].y / cellSize);
        int col = static_cast<int>(particles[i].x / cellSize);

        row = std::clamp(row, 0, rows - 1);
        col = std::clamp(col, 0, cols - 1);
        int i_int = static_cast<int>(i);

        for (int r = std::max(0, row - 1); r <= std::min(rows - 1, row + 1); r++) {
            for (int c = std::max(0, col - 1); c <= std::min(cols - 1, col + 1); c++) {
                for (int j : grid[r * cols + c]) {
                    if (j <= i_int) continue;
                    resolveCollision(particles[i], particles[j]);
                }
            }
        }
    }
}

void ParticleSimulator::resolveCollision(Particle& p1, Particle& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dist2 = dx*dx + dy*dy;
    double radiiSum = p1.radius + p2.radius;

    if (dist2 < radiiSum * radiiSum) {
        double dist = std::sqrt(dist2);
        if (dist == 0.0) {
            dx = 1e-8;
            dy = 0;
            dist = 1e-8;
        }

        double nx = dx / dist;
        double ny = dy / dist;

        double relVel = (p2.vx - p1.vx) * nx + (p2.vy - p1.vy) * ny;
        if (relVel > 0) return;

        double e = 1.0; // elastic collision
        double j = -(1 + e) * relVel / (1/p1.mass + 1/p2.mass);

        double ix = j * nx;
        double iy = j * ny;

        p1.vx -= ix / p1.mass;
        p1.vy -= iy / p1.mass;
        p2.vx += ix / p2.mass;
        p2.vy += iy / p2.mass;

        double overlap = 0.5 * (radiiSum - dist);
        p1.x -= overlap * nx;
        p1.y -= overlap * ny;
        p2.x += overlap * nx;
        p2.y += overlap * ny;
    }
}
