#pragma once
#include <functional>
#include <cmath>
#include "Particle.h"

struct Force {
    using ForceFunc = std::function<void(Particle&, Particle&, double)>;
    ForceFunc func;

    Force(ForceFunc f) : func(f) {}

    static Force gravity(double G = 1.0) {
        return Force([G](Particle &p1, Particle &p2, double dt) {
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;
            double distSqr = dx * dx + dy * dy;
            double dist = std::sqrt(distSqr);
            if (dist < 1e-5) return;

            double f = G * p1.mass * p2.mass / distSqr;
            double fx = f * dx / dist;
            double fy = f * dy / dist;

            p1.fx += fx;
            p1.fy += fy;
            p2.fx -= fx;
            p2.fy -= fy;
        });
    }

    static Force electric(double k = 1.0) {
        return Force([k](Particle &p1, Particle &p2, double dt) {
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;
            double distSqr = dx*dx + dy*dy;
            double dist = std::sqrt(distSqr);
            if (dist < 1e-5) return;

            double f = k * p1.charge * p2.charge / distSqr;
            double fx = -f * dx / dist;
            double fy = -f * dy / dist;

            p1.fx += fx;
            p1.fy += fy;
            p2.fx -= fx;
            p2.fy -= fy;
        });
    }
};
