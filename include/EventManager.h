#pragma once
#include "Interactive.h"
#include <vector>
#include "raylib.h"

class EventManager {
    std::vector<Interactive*> interactives;

public:
    EventManager() = default;

    void registerInteractive(Interactive* interactive) {
        if (interactive) {
            interactives.push_back(interactive);
        }
    }

    void pollEvents(double dt) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        bool leftPressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        for (auto* obj : interactives) {
            obj->handleEvent(dt);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
        }
    }
};