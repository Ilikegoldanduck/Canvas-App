// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include <SDL3/SDL.h>

using namespace std;

int drawMode = 1;

struct Stroke
{
	std::vector<SDL_FPoint> points;
	SDL_Color c;
	float t;

	SDL_FRect bounds;
};

struct Rect
{
	SDL_FPoint point1;
	SDL_FPoint point2;
	SDL_Color c;
	float t;

	SDL_FRect bounds;
};

struct Camera
{
	float x, y;
	float zoom;
};

struct Canvas
{
	std::vector<Stroke> lines;
	std::vector<Rect> shapes;

	Stroke currLine;
	Rect currShape;

	Camera c;
};

void renderLines(SDL_Renderer* renderer, Canvas currCanvas, const SDL_FRect& viewport)
{	
	std::vector<SDL_FPoint> transformed;
	for (const Stroke& stroke : currCanvas.lines)
	{
		if (!SDL_HasRectIntersectionFloat(&stroke.bounds, &viewport))
		{
			continue;
		}
		SDL_SetRenderDrawColor(renderer, stroke.c.r, stroke.c.g, stroke.c.b, stroke.c.a);

		transformed.clear();
		transformed.reserve(stroke.points.size());

		for (const SDL_FPoint& p : stroke.points)
		{
			transformed.push_back({
				(p.x - currCanvas.c.x),
				(p.y - currCanvas.c.y)
			});
		}
		SDL_RenderLines(renderer, transformed.data(), static_cast<int>(stroke.points.size()));
	}
}

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
	SDL_FRect rect = {std::abs(currCanvas.currShape.point1.x - p1)};
	
}

void mainMenu(SDL_Renderer* renderer)
{
	SDL_FRect rect = {0, 0, 200, 200};

	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect);
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

	mainMenu(renderer);

	renderLines(renderer, currCanvas, viewport);
	if (drawMode == 1 || drawMode == 2) {
		renderCurrLine(renderer, mouseX, mouseY, currCanvas);
	} else if (drawMode == 3) {
		renderCurrShape(renderer, mouseX, mouseY, currCanvas);
	}
			
	SDL_RenderPresent(renderer);
}

void addPoints(float p1, float p2, Canvas currCanvas)
{
	currCanvas.currLine.points.push_back(SDL_FPoint(p1, p2));
}

void endPoints(std::optional<float> p1, std::optional<float> p2, Canvas currCanvas)
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

void shapeDraw(Canvas currCanvas, SDL_Renderer* renderer, SDL_Window* window)
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
				addPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
				redrawScreen(renderer, currCanvas, window);
			}
		}

		if (drawMode != 2) {
			done = true;
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}

void freeDraw(Canvas currCanvas, SDL_Renderer* renderer, SDL_Window* window)
{
	bool done = false;

	Stroke currLine;

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
				addPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
				redrawScreen(renderer, currCanvas, window);
			}
		}

		if (drawMode != 2) {
			done = true;
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}

int main()
{

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return 1;
	}

	int wSizex, wSizey;
	wSizex = 700;
	wSizey = 500;

	SDL_Window* window = SDL_CreateWindow(
		"CanvasApp", 
		wSizex, 
		wSizey, 
		SDL_WINDOW_RESIZABLE
	);

	if (!window)
	{
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (! renderer)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_CaptureMouse(true);

	Canvas currCanvas;

	currCanvas.c.x = 0;
	currCanvas.c.y = 0;

	currCanvas.currLine.c = SDL_Color(0, 255, 0, 255);

	bool running = true;
	bool redraw = false;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	while (running)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				redraw = true;
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_Keymod modState = SDL_GetModState();
					if (drawMode == 1) {
						if (modState & SDL_KMOD_CTRL) {
							endPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
							redraw = true;
						} else {
							std::cout << std::to_string(mouseX) + "|" + std::to_string(mouseY) + "\n";
							addPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
							redraw = true;
						}
					} else if (drawMode == 2) {
						freeDraw(currCanvas, renderer, window);
					} else if (drawMode == 3) {
						shapeDraw(currCanvas, renderer, window);
					}
				}
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.key == SDLK_UP) {
					currCanvas.c.y = currCanvas.c.y - 10;
					redraw = true;
				} else if (event.key.key == SDLK_DOWN) {
					currCanvas.c.y = currCanvas.c.y + 10;
					redraw = true;
				} else if (event.key.key == SDLK_LEFT) {
					currCanvas.c.x = currCanvas.c.x - 10;
					redraw = true;
				} else if (event.key.key == SDLK_RIGHT) {
					currCanvas.c.x = currCanvas.c.x + 10;
					redraw = true;
				}
				if (event.key.key == SDLK_1) {
					drawMode = 1;
				} else if (event.key.key == SDLK_2) {
					drawMode = 2;
					endPoints(std::nullopt, std::nullopt, currCanvas);
				}
			} else {
				redraw = true;
			}
		}
		if (running && redraw) 
		{
			redrawScreen(renderer, currCanvas, window);

			redraw = false;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}