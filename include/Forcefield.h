#pragma once
#include "Area.h"
#include "Effect.h"
#include <memory>

struct Forcefield {
    std::unique_ptr<Area> area;
    std::unique_ptr<Effect> effect;

    Forcefield(std::unique_ptr<Area> a, std::unique_ptr<Effect> e)
        : area(std::move(a)), effect(std::move(e)) {}

    void apply(Particle& p, double dt) const {
        if (area->contains(p)) {
            effect->apply(p, dt);
        }
    }

    void draw() const {
        area->draw();
    }

    virtual ~Forcefield() = default;
};
