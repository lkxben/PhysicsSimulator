#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

struct Interactive {
    virtual ~Interactive() = default;
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
};
