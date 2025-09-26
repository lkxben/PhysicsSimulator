#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include "Interactive.h"
#include <iostream>

class EventManager {
    sf::RenderWindow& window;
    std::vector<Interactive*> interactives;
public:
    EventManager(sf::RenderWindow& w) : window(w) {}

    void registerInteractive(Interactive* interactive) {
        if (interactive) {
            interactives.push_back(interactive);
        }
    }

    void pollEvents(double dt) {
        while (auto eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>())
                window.close();

            for (auto* obj : interactives)
                obj->handleEvent(event, window, dt);
        }
    }
};
