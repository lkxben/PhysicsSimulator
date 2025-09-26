#include <vector>
#include <memory>
#include "../include/World.h"
#include "../include/Particle.h"
#include "../include/Simulator.h"
#include "../include/EventManager.h"
#include "../include/ForceSystem.h"
#include "../include/ForcefieldSystem.h"
#include "../include/ConstraintSystem.h"
#include "../include/CollisionSystem.h"
#include "../include/MaxDistanceConstraint.h"
#include "../include/FixedPointConstraint.h"
#include "../include/MovableFixedPointConstraint.h"
#include "../include/ElasticDistanceConstraint.h"
#include "../include/HollowCircleObstacle.h"
#include "../include/SolidCircleObstacle.h"
#include "../include/HollowPolygonObstacle.h"
#include "../include/SolidPolygonObstacle.h"
#include "../include/RenderSystem.h"
#include "../include/EulerIntegratorSystem.h"
#include "../include/VerletIntegratorSystem.h"
#include "raylib.h"

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "Test");
    SetTargetFPS(60);

    World world;

    // Systems
    Simulator simulator;
    simulator.addSystem(std::make_unique<EulerIntegratorSystem>(true, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<CollisionSystem>(world, windowWidth, windowHeight));
    simulator.addSystem(std::make_unique<ConstraintSystem>(IntegratorType::Euler));
    simulator.addSystem(std::make_unique<RenderSystem>());

    EventManager events;
    simulator.run(world, events);

    return 0;
}