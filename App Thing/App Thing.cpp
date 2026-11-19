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

struct Camera
{
	float x, y;
	float zoom;
};

void renderLines(SDL_Renderer* renderer, const std::vector<Stroke>& lines, const SDL_FRect& viewport, Camera c)
{	
	std::vector<SDL_FPoint> transformed;
	for (const Stroke& stroke : lines)
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
				(p.x - c.x),
				(p.y - c.y)
			});
		}
		SDL_RenderLines(renderer, transformed.data(), static_cast<int>(stroke.points.size()));
	}
}

void renderCurrLine(SDL_Renderer* renderer, float p1, float p2, Stroke currLine, Camera c)
{
	currLine.points.push_back(SDL_FPoint(p1 + c.x, p2 + c.y));
	SDL_SetRenderDrawColor(renderer, currLine.c.r, currLine.c.g, currLine.c.b, currLine.c.a);
	std::vector<SDL_FPoint> transformed;
	
	for (const SDL_FPoint& p : currLine.points)
	{
		transformed.push_back({
			(p.x - c.x),
			(p.y - c.y)
		});
	}
	SDL_RenderLines(renderer, transformed.data(), static_cast<int>(currLine.points.size()));
}

void mainMenu(SDL_Renderer* renderer)
{
	SDL_FRect rect = {0, 0, 200, 200};

	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void redrawScreen(SDL_Renderer* renderer, std::vector<Stroke>& lines, Stroke& currLine, SDL_Window* window, Camera c)
{
	SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
	SDL_RenderClear(renderer);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect viewport = {c.x, c.y, static_cast<float>(width), static_cast<float>(height)};

	mainMenu(renderer);

	renderLines(renderer, lines, viewport, c);
	renderCurrLine(renderer, mouseX, mouseY, currLine, c);
			
	SDL_RenderPresent(renderer);

}

void addPoints(float p1, float p2, Stroke& currLine)
{
	currLine.points.push_back(SDL_FPoint(p1, p2));
}

void endPoints(std::optional<float> p1, std::optional<float> p2, Stroke& currLine, std::vector<Stroke>& lines)
{
	if (p1 && p2) {
		currLine.points.push_back(SDL_FPoint(*p1, *p2));
	}
	currLine.c = SDL_Color(255, 0, 0, 255);
	currLine.t = 4;
	float x1, y1, x2, y2;
	x1 = currLine.points[0].x;
	y1 = currLine.points[0].y;
	x2 = x1;
	y2 = y1;
	for (const SDL_FPoint& point : currLine.points)
	{
		x1 = std::min(x1, point.x);
		x2 = std::max(x2, point.x);
		y1 = std::min(y1, point.y);
		y2 = std::max(y2, point.y);
	}
	currLine.bounds = SDL_FRect(x1, y1, x2 - x1 + 2 * currLine.t, y2 - y1 + 2 * currLine.t);
	lines.push_back(std::move(currLine));
	currLine.c = SDL_Color(0, 255, 0, 255);
}

void freeDraw(std::vector<Stroke>& lines, SDL_Renderer* renderer, SDL_Window* window, Camera c)
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
					endPoints(event.button.x + c.x, event.button.y + c.y, currLine, lines);
					done = true;	
					redrawScreen(renderer, lines, currLine, window, c);
				}
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				addPoints(mouseX + c.x, mouseY + c.y, currLine);
				redrawScreen(renderer, lines, currLine, window, c);
			}
		}

		if (drawMode != 2) {
			done = true;
			endPoints(std::nullopt, std::nullopt, currLine, lines);
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

	std::vector<Stroke> lines;
	Stroke currLine;

	currLine.c = SDL_Color(0, 255, 0, 255);

	bool running = true;
	bool redraw = false;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	Camera c;
	c.x = 0;
	c.y = 0;
	c.zoom = 1;

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
							endPoints(mouseX + c.x, mouseY + c.y, currLine, lines);
							redraw = true;
						} else {
							std::cout << std::to_string(mouseX) + "|" + std::to_string(mouseY) + "\n";
							addPoints(mouseX + c.x, mouseY + c.y, currLine);
							redraw = true;
						}
					} else if (drawMode == 2) {
						freeDraw(lines, renderer, window, c);
					}
				}
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.key == SDLK_UP) {
					c.y = c.y - 10;
					redraw = true;
				} else if (event.key.key == SDLK_DOWN) {
					c.y = c.y + 10;
					redraw = true;
				} else if (event.key.key == SDLK_LEFT) {
					c.x = c.x - 10;
					redraw = true;
				} else if (event.key.key == SDLK_RIGHT) {
					c.x = c.x + 10;
					redraw = true;
				}
				if (event.key.key == SDLK_1) {
					drawMode = 1;
				} else if (event.key.key == SDLK_2) {
					drawMode = 2;
					endPoints(std::nullopt, std::nullopt, currLine, lines);
				}
			} else {
				redraw = true;
			}
		}
		if (running && redraw) 
		{
			redrawScreen(renderer, lines, currLine, window, c);

			redraw = false;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}