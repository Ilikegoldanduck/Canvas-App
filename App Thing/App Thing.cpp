// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include <SDL3/SDL.h>

using namespace std;

struct Stroke
{
	std::vector<SDL_FPoint> points;
	SDL_Color c;
	float t;

	SDL_FRect bounds;
};

void renderLines(SDL_Renderer* renderer, const std::vector<Stroke>& lines, const SDL_FRect& viewport)
{	
	for (const Stroke& stroke : lines)
	{
		if (!SDL_HasRectIntersectionFloat(&stroke.bounds, &viewport))
		{
			continue;
		}
		SDL_SetRenderDrawColor(renderer, stroke.c.r, stroke.c.g, stroke.c.b, stroke.c.a);
		SDL_RenderLines(renderer, stroke.points.data(), static_cast<int>(stroke.points.size()));
	}
}

void mainMenu(SDL_Renderer* renderer)
{
	SDL_FRect rect = {0, 0, 200, 200};

	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void addPoints(float p1, float p2, Stroke& currLine)
{
	currLine.points.push_back(SDL_FPoint(p1, p2));
}

void endPoints(float p1, float p2, Stroke& currLine, std::vector<Stroke>& lines)
{
	currLine.points.push_back(SDL_FPoint(p1, p2));
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

	bool running = true;
	bool redraw = false;

	float mouseX, mouseY;

	while (running)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_Keymod modState = SDL_GetModState();
					if (modState & SDL_KMOD_CTRL) {
						endPoints(mouseX, mouseY, currLine, lines);
					} else {
						std::cout << std::to_string(mouseX) + "|" + std::to_string(mouseY) + "\n";
						addPoints(mouseX, mouseY, currLine);
						redraw = true;
					}
				}
			}else {
				redraw = true;
			}
		}
		if (running && redraw) 
		{
			SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
			SDL_RenderClear(renderer);

			int width, height;
			SDL_GetWindowSize(window, &width, &height);

			SDL_FRect viewport = {0, 0, static_cast<float>(width), static_cast<float>(height)};

			mainMenu(renderer);

			renderLines(renderer, lines, viewport);

			SDL_RenderPresent(renderer);
			redraw = false;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}