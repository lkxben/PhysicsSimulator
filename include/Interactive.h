#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

struct Interactive {
    virtual ~Interactive() = default;
    virtual void handleEvent(double dt) = 0;
};
