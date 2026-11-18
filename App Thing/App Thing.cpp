// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include <SDL3/SDL.h>

using namespace std;

void mainMenu()
{
	SDL_Rect rect = {100, 100, 200, 150};
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

	mainMenu();

	bool running = true;

	while (running)
	{
		SDL_Event event;

		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}