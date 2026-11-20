#pragma once

#include "App Thing.h"
#include "functions.h"

void renderCurrLine(SDL_Renderer* renderer, float p1, float p2, Canvas currCanvas)
{
	currCanvas.currLine.points.push_back(SDL_FPoint(p1 + currCanvas.c.x, p2 + currCanvas.c.y));
	SDL_SetRenderDrawColor(renderer, currCanvas.currLine.c.r, currCanvas.currLine.c.g, currCanvas.currLine.c.b, currCanvas.currLine.c.a);
	std::vector<SDL_FPoint> transformed;
	
	for (const SDL_FPoint& p : currCanvas.currLine.points)
	{
		transformed.push_back({
			(p.x - currCanvas.c.x),
			(p.y - currCanvas.c.y)
		});
	}
	SDL_RenderLines(renderer, transformed.data(), static_cast<int>(currCanvas.currLine.points.size()));
}

void renderCurrShape(SDL_Renderer* renderer, float p1, float p2, Canvas currCanvas)
{
	if (currCanvas.currShape.point1.x && currCanvas.currShape.point1.y) {
		SDL_FRect rect = {std::min(currCanvas.currShape.point1.x, p1), std::min(currCanvas.currShape.point1.y, p2), std::abs(currCanvas.currShape.point1.x - p1), std::abs(currCanvas.currShape.point1.y - p2)};
		
		SDL_SetRenderDrawColor(renderer, currCanvas.currShape.c.r, currCanvas.currShape.c.g, currCanvas.currShape.c.b, currCanvas.currShape.c.a);
		SDL_RenderRect(renderer, &rect);
	}
}

void shapeFirstPoint(float p1, float p2, Canvas& currCanvas)
{
	currCanvas.currShape.point1 = SDL_FPoint(p1 + currCanvas.c.x, p2 + currCanvas.c.y);
}

void shapeSecondPoint(float p1, float p2, Canvas& currCanvas)
{
	SDL_FPoint temp = currCanvas.currShape.point1;
	currCanvas.currShape.point1 = SDL_FPoint(std::min(temp.x, p1 + currCanvas.c.x), std::min(temp.y, p2 + currCanvas.c.y));
	currCanvas.currShape.point2 = SDL_FPoint(std::max(temp.x, p1 + currCanvas.c.x), std::max(temp.y, p2 + currCanvas.c.y));
	currCanvas.shapes.push_back(std::move(currCanvas.currShape));
}

void menu(SDL_Renderer* renderer, std::vector<menuRect> menuBox, SDL_Window* window)
{	
	for (const menuRect temp : menuBox) {
		renderSpecificMenuShape(renderer, temp, window);
	}
}

void redrawScreen(SDL_Renderer* renderer, Canvas currCanvas, SDL_Window* window)
{
	SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
	SDL_RenderClear(renderer);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect viewport = {currCanvas.c.x, currCanvas.c.y, static_cast<float>(width), static_cast<float>(height)};

	renderLines(renderer, currCanvas, viewport);
	if (currCanvas.drawMode == 1 || currCanvas.drawMode == 2) {
		renderCurrLine(renderer, mouseX, mouseY, currCanvas);
	} else if (currCanvas.drawMode == 3) {
		renderCurrShape(renderer, mouseX, mouseY, currCanvas);
	}

	menu(renderer, currCanvas.menuTable, window);
	
	SDL_RenderPresent(renderer);
}

void addPoints(float p1, float p2, Canvas& currCanvas)
{
	currCanvas.currLine.points.push_back(SDL_FPoint(p1 + currCanvas.c.x, p2 + currCanvas.c.y));
}

void endPoints(std::optional<float> p1, std::optional<float> p2, Canvas& currCanvas)
{
	if (p1 && p2) {
		currCanvas.currLine.points.push_back(SDL_FPoint(*p1, *p2));
	}
	currCanvas.currLine.c = SDL_Color(255, 0, 0, 255);
	currCanvas.currLine.t = 4;
	float x1, y1, x2, y2;
	x1 = currCanvas.currLine.points[0].x;
	y1 = currCanvas.currLine.points[0].y;
	x2 = x1;
	y2 = y1;
	for (const SDL_FPoint& point : currCanvas.currLine.points)
	{
		x1 = std::min(x1, point.x);
		x2 = std::max(x2, point.x);
		y1 = std::min(y1, point.y);
		y2 = std::max(y2, point.y);
	}
	currCanvas.currLine.bounds = SDL_FRect(x1, y1, x2 - x1 + 2 * currCanvas.currLine.t, y2 - y1 + 2 * currCanvas.currLine.t);
	currCanvas.lines.push_back(std::move(currCanvas.currLine));
	currCanvas.currLine.c = SDL_Color(0, 255, 0, 255);
}

void shapeDraw(Canvas& currCanvas, SDL_Renderer* renderer, SDL_Window* window)
{
	bool done = false;

	while (! done)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					shapeSecondPoint(event.button.x, event.button.y, currCanvas);
					done = true;
					redrawScreen(renderer, currCanvas, window);
				}
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				redrawScreen(renderer, currCanvas, window);
			}
		}
	}
}

void freeDraw(Canvas& currCanvas, SDL_Renderer* renderer, SDL_Window* window)
{
	bool done = false;

	float mouseX, mouseY;

	while (! done)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					endPoints(event.button.x + currCanvas.c.x, event.button.y + currCanvas.c.y, currCanvas);
					done = true;	
					redrawScreen(renderer, currCanvas, window);
				}
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				addPoints(mouseX, mouseY, currCanvas);
				redrawScreen(renderer, currCanvas, window);
			}
		}

		if (currCanvas.drawMode != 2) {
			done = true;
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}

void changeState(SDL_Event event, int& drawMode, bool& changeMode, Canvas currCanvas)
{
	if (event.key.key == SDLK_1) {
		drawMode = 1;
		changeMode = true;
	} else if (event.key.key == SDLK_2) {
		drawMode = 2;
		changeMode = true;
	} else if (event.key.key == SDLK_3) {
		drawMode = 3;
		changeMode = true;
	}
	if (changeMode) {
		if (!currCanvas.currLine.points.empty()) {
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}
