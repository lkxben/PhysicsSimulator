#include <vector>
#include <memory>
#include <random>
#include "../include/World.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/CollisionSystem.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"
#include "../include/SolidRectObstacle.h"
#include "../include/SolidPolygonObstacle.h"
#include "../include/HollowRectObstacle.h"
#include "../include/LaunchableParticle.h"
#include "Ball.h"
#include "Pocket.h"

int main() {
    const unsigned int windowWidth = 1000;
    const unsigned int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "Pool");
    SetTargetFPS(200);

    World world;

    // Set up balls
    struct Vec2 { double x, y; };
    double xShift = 180;
    Vec2 balls[] = {
        {500 + xShift, 300},
        {516 + xShift, 292}, {516 + xShift, 308},
        {532 + xShift, 284}, {532 + xShift, 300}, {532 + xShift, 316},
        {548 + xShift, 276}, {548 + xShift, 292}, {548 + xShift, 308}, {548 + xShift, 324}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posOffset(-0.7, 0.7);

    for (size_t i = 0; i < std::size(balls); ++i) {
        double x = balls[i].x + posOffset(gen);
        double y = balls[i].y + posOffset(gen);
        Color color = (i % 2 == 0) ? Color{255, 140, 0, 255} : Color{0, 255, 255, 255};
        world.particles.push_back(std::make_unique<Ball>(x, y, 0.0, 0.0, 1.0, 8.0, 0.9, color));
    }

    // Horizontal cushions
    float cushionThickness = 8.0f;
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(312.0, 108.0, 328.0, cushionThickness));
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(688.0, 108.0, 328.0, cushionThickness));
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(312.0, 492.0, 328.0, cushionThickness));
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(688.0, 492.0, 328.0, cushionThickness));

    // Vertical cushions
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(128.0, 300.0, 344.0, cushionThickness, M_PI / 2));
    world.obstacles.push_back(std::make_unique<SolidRectObstacle>(872.0, 300.0, 344.0, cushionThickness, M_PI / 2));

    // Pocket jaws
    auto makeTriangle = [](float x1, float y1, float x2, float y2, float tipX, float tipY) {
        return std::make_unique<SolidPolygonObstacle>(
            std::vector<Vector2>{{x1,y1}, {x2,y2}, {tipX,tipY}}, 0.9, WHITE
        );
    };
    world.obstacles.push_back(makeTriangle(140, 104, 148, 112, 148, 104));
    world.obstacles.push_back(makeTriangle(476, 104, 476, 112, 484, 104));
    world.obstacles.push_back(makeTriangle(516, 104, 524, 112, 524, 104));
    world.obstacles.push_back(makeTriangle(852, 104, 852, 112, 860, 104));
    world.obstacles.push_back(makeTriangle(140, 496, 148, 496, 148, 488));
    world.obstacles.push_back(makeTriangle(476, 488, 476, 496, 484, 496));
    world.obstacles.push_back(makeTriangle(516, 496, 524, 496, 524, 488));
    world.obstacles.push_back(makeTriangle(852, 488, 852, 496, 860, 496));
    world.obstacles.push_back(makeTriangle(124, 120, 124, 128, 132, 128));
    world.obstacles.push_back(makeTriangle(124, 472, 124, 480, 132, 472));
    world.obstacles.push_back(makeTriangle(876, 120, 868, 128, 876, 128));
    world.obstacles.push_back(makeTriangle(868, 472, 876, 480, 876, 472));

    // Table
    world.obstacles.push_back(std::make_unique<HollowRectObstacle>(500, 300, 792, 432, 20.0));

    // Pockets
    float pocketRadius = 16.0f;
    world.obstacles.push_back(std::make_unique<Pocket>(124, 108.0 - 4.0, pocketRadius));
    world.obstacles.push_back(std::make_unique<Pocket>(500, 108.0 - 4.0, pocketRadius));
    world.obstacles.push_back(std::make_unique<Pocket>(872 + 4.0, 108.0 - 4.0, pocketRadius));
    world.obstacles.push_back(std::make_unique<Pocket>(128 - 4.0, 492 + 4.0, pocketRadius));
    world.obstacles.push_back(std::make_unique<Pocket>(500, 492 + 4.0, pocketRadius));
    world.obstacles.push_back(std::make_unique<Pocket>(872 + 4.0, 492 + 4.0, pocketRadius));

    // Forcefields
    world.forcefields.push_back(std::make_unique<Forcefield>(
        std::make_unique<RectArea>(500, 300, 736, 376, Color{0,102,204,255}),
        std::make_unique<DragEffect>(0.50)
    ));

    // Launchable particle
    EventManager events;
    auto particle = std::make_unique<LaunchableParticle>(300, 300, 1.0, 8.0, 0.9, WHITE, 2000);
    events.registerInteractive(particle.get());
    world.particles.push_back(std::move(particle));

    // Systems
    Simulator simulator;
    simulator.addSystem(std::make_unique<ForcefieldSystem>());
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<CollisionSystem>(world, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<RenderSystem>());

    // Run simulation
    simulator.run(world, events);

    return 0;
}