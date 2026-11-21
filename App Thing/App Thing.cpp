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

	totalWindow renderWindow;
	Canvas& currCanvas = renderWindow.currCanvas;

	currCanvas.drawMode = 1;

	currCanvas.c.x = 0;
	currCanvas.c.y = 0;

	currCanvas.currLine.c = SDL_Color(0, 255, 0, 255);

	currCanvas.currShape = Rect();
	currCanvas.currShape.c = SDL_Color(0, 255, 0, 255);
	currCanvas.currShape.round = 0;

	bool running = true;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	guiWindow drawTab;

	createDrawWindow(drawTab);

	renderWindow.windows.push_back(drawTab);

	redrawScreen(renderer, renderWindow, window);

	while (running)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_GetMouseState(&mouseX, &mouseY);
					if (checkMenuCollision(mouseX, mouseY, renderWindow, window, renderer)) {
						std::cout << "done!";
					} else if (currCanvas.drawMode == 1) {
						lineDraw(renderWindow, renderer, window, running);
					} else if (currCanvas.drawMode == 2) {
						freeDraw(renderWindow, renderer, window, running);
					} else if (currCanvas.drawMode == 3) {
						shapeFirstPoint(mouseX, mouseY, currCanvas);
						shapeDraw(renderWindow, renderer, window, running);
					}
				}
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				cameraChange(renderWindow, event, renderer, window);
				bool changeMode = false;
				changeState(event, changeMode, currCanvas);
			}

			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}