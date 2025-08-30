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
    if (auto solidRect = dynamic_cast<SolidRectObstacle*>(&o)) {
        check(p, *solidRect);
    } else if (auto solidCircle = dynamic_cast<SolidCircleObstacle*>(&o)) {
        check(p, *solidCircle);
    }
}

void CollisionChecker::check(Particle& p, SolidRectObstacle& r) {
    double cosA = std::cos(r.rotation);
    double sinA = std::sin(r.rotation);

    // Translate particle position to rectangle local coordinates
    double relX = p.x - r.x;
    double relY = p.y - r.y;

    // Rotate particle position into rectangle's axis-aligned frame
    double localX =  cosA * relX + sinA * relY;
    double localY = -sinA * relX + cosA * relY;

    // Clamp to rectangle half extents
    double halfW = r.width / 2.0;
    double halfH = r.height / 2.0;
    double clampedX = std::clamp(localX, -halfW, halfW);
    double clampedY = std::clamp(localY, -halfH, halfH);

    // Closest point in world coordinates
    double closestX = cosA * clampedX - sinA * clampedY + r.x;
    double closestY = sinA * clampedX + cosA * clampedY + r.y;

    // Vector from closest point to particle
    double dx = p.x - closestX;
    double dy = p.y - closestY;
    double dist2 = dx*dx + dy*dy;

    if (dist2 < p.radius * p.radius) {
        double dist = std::sqrt(dist2);
        if (dist == 0.0) { dx = 1e-8; dy = 0; dist = 1e-8; }

        double nx = dx / dist;
        double ny = dy / dist;

        double overlap = p.radius - dist;
        p.x += nx * overlap;
        p.y += ny * overlap;

        double vDotN = p.vx * nx + p.vy * ny;
        if (vDotN < 0) {
            double e = 0.5 * (p.elasticity + r.elasticity);
            p.vx -= (1 + e) * vDotN * nx;
            p.vy -= (1 + e) * vDotN * ny;
        }
    }
}


void CollisionChecker::check(Particle& p, SolidCircleObstacle& c) {
    double dx = p.x - c.x;
    double dy = p.y - c.y;
    double dist2 = dx*dx + dy*dy;
    double radiiSum = p.radius + c.radius;

    if (dist2 < radiiSum * radiiSum) {
        double dist = std::sqrt(dist2);
        if (dist == 0.0) { 
            dx = 1e-8; dy = 0; 
            dist = 1e-8; 
        }

        double nx = dx / dist;
        double ny = dy / dist;

        double relVel = p.vx * nx + p.vy * ny;

        if (relVel < 0) {
            double e = 0.5 * (p.elasticity + c.elasticity);

            p.vx -= (1 + e) * relVel * nx;
            p.vy -= (1 + e) * relVel * ny;
        }

        double overlap = radiiSum - dist;
        p.x += nx * overlap;
        p.y += ny * overlap;
    }
}
