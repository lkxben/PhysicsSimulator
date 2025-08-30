#include "CollisionChecker.h"
#include <cmath>

void CollisionChecker::check(Entity* a, Entity* b) {
    if (auto p1 = dynamic_cast<Particle*>(a)) {
        if (auto p2 = dynamic_cast<Particle*>(b)) {
            check(p1, p2);
        } else if (auto o = dynamic_cast<Obstacle*>(b)) {
            check(p1, o);
        }
    }
}

void CollisionChecker::check(Particle& p1, Particle& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dist2 = dx*dx + dy*dy;
    double radiiSum = p1.radius + p2.radius;

    if (dist2 < radiiSum * radiiSum) {
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

        double overlap = 0.5 * (radiiSum - dist);
        p1.x -= overlap * nx;
        p1.y -= overlap * ny;
        p2.x += overlap * nx;
        p2.y += overlap * ny;
    }
}

void CollisionChecker::check(Particle& p, Obstacle& o) {
    o.collide(p);
}