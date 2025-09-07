#pragma once

struct Constraint {
    virtual void apply(double dt) = 0;
    virtual ~Constraint() = default;
};
