#pragma once

#include "App Thing.h"
#include "functions.h"

Rect createRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r)
{
	Rect temp;
	temp.point1 = p1;
	temp.point2 = p2;
	temp.c = c;
	temp.t = t;
	temp.round = r;

	createRoundedRectangle(temp);

	return temp;
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

	std::cout << std::to_string(cx) + "\n";
	std::cout << std::to_string(cy) + "\n";
	return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
}

bool checkRoundedMenuCollision(float p1, float p2, guiWindow drawWindow, SDL_Window* window)
{
	Rect rectangle = drawWindow.body;

	SDL_FPoint pt1 = rectangle.point1;
	SDL_FPoint pt2 = rectangle.point2;
	
	std::cout << std::to_string(p1) + "\n";
	std::cout << std::to_string(p2) + "\n";

	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
		rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
	}

	float cx = std::max(std::min(pt1.x, pt2.x) + rad + drawWindow.x, std::min(p1, std::max(pt1.x, pt2.x) - rad + drawWindow.x));
	float cy = std::max(std::min(pt1.y, pt2.y) + rad + drawWindow.y, std::min(p2, std::max(pt1.y, pt2.y) - rad + drawWindow.y));

	std::cout << std::to_string(cx) + "\n";
	std::cout << std::to_string(cy) + "\n";

	return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
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

void moveDrawTabGuiWindow(SDL_Renderer* renderer, totalWindow& renderWindow, SDL_Window* window, float x, float y)
{
    bool done = false;

	float windowX = renderWindow.drawTab.x;
	float windowY = renderWindow.drawTab.y;

    while (! done) {
        SDL_Event event;

        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
                done = true;
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				renderWindow.drawTab.x = windowX + (event.motion.x - x);
				renderWindow.drawTab.y = windowY + (event.motion.y - y);
				std::cout << std::to_string(renderWindow.drawTab.x) + "|" + std::to_string(renderWindow.drawTab.y) + "\n";
				redrawScreen(renderer, renderWindow, window);
			}
        }
    }
};

void createDrawWindow(guiWindow& drawWindow)
{
    drawWindow.body = createRect(SDL_FPoint(0, 0), SDL_FPoint(50, 200), SDL_Color(255, 0, 0, 255), 1, 25);
    drawWindow.shapes.push_back(createRect(SDL_FPoint(0, 25), SDL_FPoint(25, 200), SDL_Color(255, 0, 0, 255), 1, 20));

	drawWindow.x = 0;
	drawWindow.y = 0;
}	

void createMenu(Canvas& currCanvas)
{	//Might change design to be more like a bar that you can just move around idk
	
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(0, 10), SDL_FPoint(300, 110), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(100, 60), SDL_FPoint(300, 300), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
}
