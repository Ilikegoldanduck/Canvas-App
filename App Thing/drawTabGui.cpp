#pragma once

#include "App Thing.h"
#include "functions.h"

bool drawTab_checkGuiCollision(guiWindow guiWindow, SDL_Renderer* renderer, SDL_Window* window, totalWindow& renderWindow)
{
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (Button button : guiWindow.buttons) {
        Rect rectangle = button.rect;
        if (checkButtonCollision(mouseX, mouseY, rectangle, guiWindow)) {
            if (button.name == "lineDraw") {
                renderWindow.currCanvas.drawMode = 1;
            } else if (button.name == "freeDraw") {
                renderWindow.currCanvas.drawMode = 2;
            } else if (button.name == "shapeDraw") {
                renderWindow.currCanvas.drawMode = 3;
            }
            return true;
        }
    }
    return false;
}