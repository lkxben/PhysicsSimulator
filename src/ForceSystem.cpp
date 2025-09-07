#include "ForceSystem.h"
#include <cmath>

void ForceSystem::apply(const std::vector<std::unique_ptr<Particle>>& particles, double dt) {
    for (auto &p : particles) {
        p->fx = 0;
        p->fy = 0;
    }

    for (auto &force : forces) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                force.func(*particles[i], *particles[j], dt);
            }
        }
    }

    for (auto &p : particles) {
        p->vx += (p->fx / p->mass) * dt;
        p->vy += (p->fy / p->mass) * dt;
        p->x += p->vx * dt;
        p->y += p->vy * dt;
    }
}
