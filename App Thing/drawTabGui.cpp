#pragma once

#include "App Thing.h"
#include "functions.h"

bool checkButtonCollision(float p1, float p2, const Rect& rectangle, const guiWindow guiWindow)
{
    SDL_FPoint pt1 = rectangle.point1;
    SDL_FPoint pt2 = rectangle.point2;

    float rad = rectangle.round;
    if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
        rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
    }

    float cx = std::max(std::min(pt1.x, pt2.x) + rad + guiWindow.x, std::min(p1, std::max(pt1.x, pt2.x) - rad + guiWindow.x));
    float cy = std::max(std::min(pt1.y, pt2.y) + rad + guiWindow.y, std::min(p2, std::max(pt1.y, pt2.y) - rad + guiWindow.y));

    return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
}

bool drawTab_checkGuiCollision(guiWindow guiWindow, SDL_Renderer* renderer, SDL_Window* window)
{
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (Button button : guiWindow.buttons) {
        Rect rectangle = button.rect;
        if (checkButtonCollision(mouseX, mouseY, rectangle, guiWindow)) {
            return true;
        }
    }
    return false;
}