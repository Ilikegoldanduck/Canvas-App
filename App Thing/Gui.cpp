#pragma once

#include "App Thing.h"
#include "functions.h"

using namespace std;

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

Rect createRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r)
{
	Rect temp;
	temp.point1 = p1;
	temp.point2 = p2;
	temp.c = c;
	temp.t = t;
	temp.round = r;
	temp.bounds = SDL_FRect(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::abs(p1.x - p2.x), std::abs(p1.y - p2.y));

	createRoundedRectangle(temp);

	return temp;
}

Button createButton(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r, std::string name, std::string text)
{
	Button button;
	
	Rect temp;
	temp.point1 = p1;
	temp.point2 = p2;
	temp.c = c;
	temp.t = t;
	temp.round = r;
	temp.bounds = SDL_FRect(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::abs(p1.x - p2.x), std::abs(p1.y - p2.y));
	
	createRoundedRectangle(temp);

	button.rect = temp;
	button.name = name;
	button.text = text;

	return button;
}

menuRect createMenuRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r, float offsetX, float offsetY)
{
	menuRect temp;
	temp.rect = createRect(p1, p2, c, t, r);
	temp.offsetX = offsetX;
	temp.offsetY = offsetY;

	return temp;
}

void renderMenu(guiWindow guiWindow, SDL_Renderer* renderer, SDL_Window* window)
{
	Rect rect = guiWindow.body;
	std::vector<SDL_FPoint> temp;
	for (SDL_FPoint temptemp : rect.completedShape) {
		temptemp.x = temptemp.x + guiWindow.x;
		temptemp.y = temptemp.y + guiWindow.y;
		temp.push_back(temptemp);
	}
	SDL_SetRenderDrawColor(renderer, rect.c.r, rect.c.g, rect.c.b, rect.c.a);
	SDL_RenderLines(renderer, temp.data(), static_cast<int>(temp.size()));

	for (Rect menuObj : guiWindow.shapes) {
		std::vector<SDL_FPoint> temp;

		SDL_SetRenderDrawColor(renderer, menuObj.c.r, menuObj.c.g, menuObj.c.b, menuObj.c.a);
		for (SDL_FPoint temptemp : menuObj.completedShape) {	
			temptemp.x = temptemp.x + guiWindow.x;
			temptemp.y = temptemp.y + guiWindow.y;
			temp.push_back(temptemp);
		}
		SDL_RenderLines(renderer, temp.data(), static_cast<int>(temp.size()));
	}
	
	for (Button menuButton : guiWindow.buttons) {
		Rect menuObj = menuButton.rect;
		std::vector<SDL_FPoint> temp;

		SDL_SetRenderDrawColor(renderer, menuObj.c.r, menuObj.c.g, menuObj.c.b, menuObj.c.a);
		for (SDL_FPoint temptemp : menuObj.completedShape) {	
			temptemp.x = temptemp.x + guiWindow.x;
			temptemp.y = temptemp.y + guiWindow.y;
			temp.push_back(temptemp);
		}

		
		//SDL_RenderDebugText(renderer, menuButton.rect.point1.x, menuButton.rect.point1.y, menuButton.text);

		SDL_RenderLines(renderer, temp.data(), static_cast<int>(temp.size()));
	}
}

bool checkRoundedCollision(float p1, float p2, Rect rectangle)
{
	SDL_FPoint pt1 = rectangle.point1;
	SDL_FPoint pt2 = rectangle.point2;
	
	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
		rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
	}

	float cx = std::max(std::min(pt1.x, pt2.x) + rad, std::min(p1, std::max(pt1.x, pt2.x) - rad));
	float cy = std::max(std::min(pt1.y, pt2.y) + rad, std::min(p2, std::max(pt1.y, pt2.y) - rad));

	return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
}

bool checkMenuCollision(float p1, float p2, totalWindow& renderWindow, SDL_Window* window, SDL_Renderer* renderer)
{
	for (guiWindow& drawWindow : renderWindow.windows) {
		Rect rectangle = drawWindow.body;

		SDL_FPoint pt1 = rectangle.point1;
		SDL_FPoint pt2 = rectangle.point2;

		float rad = rectangle.round;
		if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
			rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
		}

		float cx = std::max(std::min(pt1.x, pt2.x) + rad + drawWindow.x, std::min(p1, std::max(pt1.x, pt2.x) - rad + drawWindow.x));
		float cy = std::max(std::min(pt1.y, pt2.y) + rad + drawWindow.y, std::min(p2, std::max(pt1.y, pt2.y) - rad + drawWindow.y));

		if (std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round) {
			if (drawWindow.name == "drawTab" && drawTab_checkGuiCollision(drawWindow, renderer, window, renderWindow)) {
				return true;
			}
			moveDrawTabGuiWindow(renderer, renderWindow, drawWindow, window, p1, p2);
			return true;
		}
	}
	return false;
}

void renderSpecificMenuShape(SDL_Renderer* renderer, menuRect rect, SDL_Window* window)
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	
	std::vector<SDL_FPoint> temp;
	Rect rectangle = rect.rect;

	SDL_SetRenderDrawColor(renderer, rectangle.c.r, rectangle.c.g, rectangle.c.b, rectangle.c.a);
	for (SDL_FPoint temptemp : rectangle.completedShape) {
		temptemp.x = (width / 2 - std::abs(rectangle.point1.x - rectangle.point2.x) + temptemp.x); //uhh so basically middle to the right to the half of the width of the rectangle
		temp.push_back(temptemp);
	}
	SDL_RenderLines(renderer, temp.data(), static_cast<int>(temp.size()));
}

void moveDrawTabGuiWindow(SDL_Renderer* renderer, totalWindow& renderWindow, guiWindow& drawTab, SDL_Window* window, float x, float y)
{
    bool done = false;

	float windowX = drawTab.x;
	float windowY = drawTab.y;

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

    while (! done) {
        SDL_Event event;

        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
                done = true;
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				drawTab.x = std::max(0.0f, std::min(static_cast<float>(width - drawTab.body.point2.x), windowX + (event.motion.x - x)));
				drawTab.y = std::max(0.0f, std::min(static_cast<float>(height - drawTab.body.point2.y), windowY + (event.motion.y - y)));
				redrawScreen(renderer, renderWindow, window);
			}
        }
    }
};

void createDrawWindow(guiWindow& drawWindow)
{
	drawWindow.name = "drawTab";

    drawWindow.body = createRect(SDL_FPoint(0, 0), SDL_FPoint(50, 250), SDL_Color(255, 0, 0, 255), 1, 25);
    drawWindow.shapes.push_back(createRect(SDL_FPoint(0, 50), SDL_FPoint(50, 250), SDL_Color(255, 0, 0, 255), 1, 20));

	drawWindow.buttons.push_back(createButton(SDL_FPoint(0, 50), SDL_FPoint(50, 100), SDL_Color(255, 0, 0, 255), 1, 20, "lineDraw", ""));
	drawWindow.buttons.push_back(createButton(SDL_FPoint(0, 100), SDL_FPoint(50, 150), SDL_Color(255, 0, 0, 255), 1, 20, "freeDraw", ""));
	drawWindow.buttons.push_back(createButton(SDL_FPoint(0, 150), SDL_FPoint(50, 200), SDL_Color(255, 0, 0, 255), 1, 20, "shapeDraw", ""));
	drawWindow.buttons.push_back(createButton(SDL_FPoint(0, 200), SDL_FPoint(50, 250), SDL_Color(255, 0, 0, 255), 1, 20, "erase", ""));

	drawWindow.x = 0;
	drawWindow.y = 0;
}