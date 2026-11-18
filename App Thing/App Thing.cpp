// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include <SDL3/SDL.h>

using namespace std;

typedef struct Color
{
	int r;
	int g;
	int b;
	int a;
} Color;

typedef struct Line
{
	float x1, y1;
	float x2, y2;
	Color c;
} Line;

void mainMenu(SDL_Renderer* renderer)
{
	SDL_FRect rect = {100, 100, 200, 150};

	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect);
}

int main()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"CanvasApp", 
		700, 
		500, 
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

	bool running = true;
	bool redraw = false;

	while (running)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			} else {
				redraw = true;
			}
		}
		if (running && redraw) 
		{
			SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
			SDL_RenderClear(renderer);

			mainMenu(renderer);


			SDL_RenderPresent(renderer);
			redraw = false;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}