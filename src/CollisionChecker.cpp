#include "CollisionChecker.h"
#include <cmath>

void CollisionChecker::check(Entity* a, Entity* b, double dt) {
    if (auto p1 = dynamic_cast<Particle*>(a)) {
        if (auto p2 = dynamic_cast<Particle*>(b)) {
            check(p1, p2, dt);
        } else if (auto o = dynamic_cast<Obstacle*>(b)) {
            check(p1, o, dt);
        }
    }
}

void CollisionChecker::check(Particle& p1, Particle& p2, double dt) {
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

void CollisionChecker::check(Particle& p, Obstacle& o, double dt) {
    o.collide(p, dt);
}
