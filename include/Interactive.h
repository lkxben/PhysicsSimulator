#pragma once

struct Interactive {
    virtual ~Interactive() = default;
    virtual void handleEvent(double dt) = 0;
};
