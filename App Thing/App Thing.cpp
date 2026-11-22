// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "App Thing.h"
#include "functions.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

void quit(SDL_Window* window, const Canvas& currCanvas)
{
	std::ofstream file("canvasSave.txt");
	
	if (!file) {
		SDL_Log("Failed to open save file!");
		return;
	}

	for (Stroke strokes : currCanvas.lines) {
		file << "ss";
		for (SDL_FPoint points : strokes.points) {
			file << std::to_string(points.x) + "'" + to_string(points.y) + "|";
		}
		file << "se" << std::to_string(strokes.c.r) + "'" + std::to_string(strokes.c.g) + "'" + std::to_string(strokes.c.b) + "'" + std::to_string(strokes.c.a) + "'" + std::to_string(strokes.t) + "\n";
	}
	for (Rect rect : currCanvas.shapes) {
		file << "rs";
		file << std::to_string(rect.point1.x) + "'" + std::to_string(rect.point1.y) + "|";
		file << std::to_string(rect.point2.x) + "'" + std::to_string(rect.point2.y) + "|";
		file << "re" << std::to_string(rect.c.r) + "'" + std::to_string(rect.c.g) + "'" + std::to_string(rect.c.b) + "'" + std::to_string(rect.c.a) + "'" + std::to_string(rect.t) + "'" + std::to_string(rect.round) + "\n";
	}

	file.close();

	SDL_DestroyWindow(window);

	SDL_Quit();
}

void load(Canvas& currCanvas, std::string chosenFile)
{
	std::ifstream file(chosenFile);

	if (!file) {
		SDL_Log("No save found!");
		return;
	}

	std::string line;

	while (std::getline(file, line)) {
		if (line.starts_with("ss")) {
			Stroke stroke;

			size_t end = line.find("se", 2);

			std::string pointsData = line.substr(2, end - 2);
			std::stringstream pointsStream(pointsData);

			std::string point;

			while (std::getline(pointsStream, point, '|')) {
				std::stringstream pointStream(point);

				std::string x, y;

				std::getline(pointStream, x, '\'');
				std::getline(pointStream, y);

				stroke.points.push_back(SDL_FPoint(std::stof(x), std::stof(y)));
			}

			std::string props = line.substr(end + 2);

			std::stringstream propStream(props);

			std::string vals;
			std::vector<float> values;

			while (std::getline(propStream, vals, '\'')) {
				values.push_back(std::stof(vals));
			}

			stroke.c.r = values[0];
			stroke.c.g = values[1];
			stroke.c.b = values[2];
			stroke.c.a = values[3];
			stroke.t = values[4];

		} else if (line.starts_with("rs")) {
			Rect rect;

			size_t end = line.find("re", 2);

			if (end == std::string::npos) {
				continue;
			}


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

	totalWindow renderWindow;
	Canvas& currCanvas = renderWindow.currCanvas;

	currCanvas.drawMode = 1;

	currCanvas.c.x = 0;
	currCanvas.c.y = 0;

	currCanvas.currLine.c = SDL_Color(0, 255, 0, 255);

	currCanvas.currShape = Rect();
	currCanvas.currShape.c = SDL_Color(0, 255, 0, 255);
	currCanvas.currShape.round = 0;

	load(currCanvas, "canvasSave.txt");

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

		if (SDL_WaitEvent(&event) && SDL_WINDOW_MOUSE_FOCUS && SDL_GetWindowFlags(window)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

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
		}
	}
	return 0;
}