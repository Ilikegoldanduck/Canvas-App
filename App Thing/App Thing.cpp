// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include "functions.h"
#include <SDL3/SDL.h>

using namespace std;

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

	currCanvas.drawMode = 1;

	currCanvas.c.x = 0;
	currCanvas.c.y = 0;

	currCanvas.currLine.c = SDL_Color(0, 255, 0, 255);
	currCanvas.currShape.c = SDL_Color(0, 255, 0, 255);
	
	createMenu(currCanvas);

	bool running = true;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	while (running)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_Keymod modState = SDL_GetModState();
					if (checkRoundedMenuCollision(mouseX, mouseY, currCanvas.menuTable[0], window)) {
						std::cout << "done!";
					} else if (currCanvas.drawMode == 1) {
						lineDraw(currCanvas, renderer, window);
					} else if (currCanvas.drawMode == 2) {
						freeDraw(currCanvas, renderer, window);
					} else if (currCanvas.drawMode == 3) {
						shapeFirstPoint(mouseX, mouseY, currCanvas);
						shapeDraw(currCanvas, renderer, window);
					}
				}
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				cameraChange(currCanvas, event, renderer, window);
				bool changeMode = false;
				changeState(event, currCanvas.drawMode, changeMode, currCanvas);
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}