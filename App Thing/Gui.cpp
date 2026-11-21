#pragma once

#include "App Thing.h"
#include "functions.h"

void createRoundedRectangle(Rect& rectangle)
{
	std::vector<SDL_FPoint> temp;

	SDL_FPoint p1 = rectangle.point1;
	SDL_FPoint p2 = rectangle.point2;

	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(p1.x - p2.x) / 2, std::abs(p1.y - p2.y) / 2)) {
		rad = std::min(std::abs(p1.x - p2.x) / 2, std::abs(p1.y - p2.y) / 2);
	}
	
	SDL_FPoint corners[4] = {
		SDL_FPoint(std::max(p1.x, p2.x) - rad, std::max(p1.y, p2.y) - rad),
		SDL_FPoint(std::min(p1.x, p2.x) + rad, std::max(p1.y, p2.y) - rad),
		SDL_FPoint(std::min(p1.x, p2.x) + rad, std::min(p1.y, p2.y) + rad),
		SDL_FPoint(std::max(p1.x, p2.x) - rad, std::min(p1.y, p2.y) + rad)
	};

	for (int c = 0; c < 4; c++) {
		for (int i = 0; i < 90; i++) {
			float theta = (90 * c + i) * std::numbers::pi_v<float> / 180.0f;
			SDL_FPoint pt = SDL_FPoint(corners[c].x + (rad * std::cos(theta)), corners[c].y + (rad * std::sin(theta)));
			temp.push_back({pt});
		}
	}

	temp.push_back(SDL_FPoint(std::max(p1.x, p2.x), std::max(p1.y, p2.y) - rad));

	rectangle.completedShape = temp;
}

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

bool checkRoundedMenuCollision(float p1, float p2, menuRect rect, SDL_Window* window)
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	Rect rectangle = rect.rect;

	SDL_FPoint pt1 = rectangle.point1;
	SDL_FPoint pt2 = rectangle.point2;
	
	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
		rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
	}

	float cx = std::max(width / 2 - std::abs(rectangle.point1.x - rectangle.point2.x) + std::min(pt1.x, pt2.x) + rad, width / 2 - std::abs(rectangle.point1.x - rectangle.point2.x) + std::min(p1, std::max(pt1.x, pt2.x) - rad));
	float cy = std::max(std::min(pt1.y, pt2.y) + rad, std::min(p2, std::max(pt1.y, pt2.y) - rad));

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

void createMenu(Canvas& currCanvas)
{	//Might change design to be more like a bar that you can just move around idk
	
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(0, 10), SDL_FPoint(300, 110), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(100, 60), SDL_FPoint(300, 300), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
}
