// CMakeProject1.cpp : Defines the entry point for the application.

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
		if (strokes.points.size() > 1)
		{
			file << "ss";
			for (SDL_FPoint points : strokes.points) {
				file << std::to_string(points.x) + "'" + to_string(points.y) + "|";
			}
			file << "se" << std::to_string(strokes.c.r) + "'" + std::to_string(strokes.c.g) + "'" + std::to_string(strokes.c.b) + "'" + std::to_string(strokes.c.a) + "'" + std::to_string(strokes.t) + "\n";
		}
	}
	for (Rect rect : currCanvas.shapes) {
		file << "rs";
		file << std::to_string(rect.point1.x) + "'" + std::to_string(rect.point1.y) + "|";
		file << std::to_string(rect.point2.x) + "'" + std::to_string(rect.point2.y) + "|";
		file << "re" << std::to_string(rect.c.r) + "'" + std::to_string(rect.c.g) + "'" + std::to_string(rect.c.b) + "'" + std::to_string(rect.c.a) + "'" + std::to_string(rect.t) + "'" + std::to_string(rect.round) + "\n";
	}

	file.close();

	TTF_Quit();

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

			if (end == std::string::npos) {
				continue;
			}

			std::string pointsData = line.substr(2, end - 2);
			std::stringstream pointsStream(pointsData);

			std::string point;

			float x1, y1, x2, y2;

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


			x1 = stroke.points[0].x;
			y1 = stroke.points[0].y;
			x2 = x1;
			y2 = y1;
			for (const SDL_FPoint& point : stroke.points)
			{
				x1 = std::min(x1, point.x);
				x2 = std::max(x2, point.x);
				y1 = std::min(y1, point.y);
				y2 = std::max(y2, point.y);
			}
			
			stroke.bounds = SDL_FRect(x1, y1, x2 - x1 + 2 * stroke.t, y2 - y1 + 2 * stroke.t);

			currCanvas.lines.push_back(stroke);
		} else if (line.starts_with("rs")) {

			size_t end = line.find("re", 2);

			if (end == std::string::npos) {
				continue;
			}

			std::string pointsData = line.substr(2, end - 2);
			std::stringstream pointsStream(pointsData);

			std::string point1;
			std::string point2;

			std::getline(pointsStream, point1, '|');
			std::getline(pointsStream, point2);

			std::stringstream point1Stream(point1);
			std::stringstream point2Stream(point2);

			std::string x1, y1;
			std::string x2, y2;

			std::getline(point1Stream, x1, '\'');
			std::getline(point1Stream, y1);
			std::getline(point2Stream, x2, '\'');
			std::getline(point2Stream, y2);

			std::string props = line.substr(end + 2);

			std::stringstream propStream(props);

			std::string vals;
			std::vector<float> values;

			while (std::getline(propStream, vals, '\'')) {
				values.push_back(std::stof(vals));
			}

			Rect rect;

			if (values[4] == 0) {
				rect = createRect(SDL_FPoint(std::stof(x1), std::stof(y1)), SDL_FPoint(std::stof(x2), std::stof(y2)), SDL_Color(values[0], values[1], values[2], values[3]), 1, values[5]);
			} else {
				rect = createRect(SDL_FPoint(std::stof(x1), std::stof(y1)), SDL_FPoint(std::stof(x2), std::stof(y2)), SDL_Color(values[0], values[1], values[2], values[3]), values[4], values[5]);
			}

			currCanvas.shapes.push_back(rect);
		}
	}
}

int main()
{
	if (! SDL_Init(SDL_INIT_VIDEO))
	{
		return 1;
	}

	if (! TTF_Init())
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
	currCanvas.currShape.t = 3;

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

		if (SDL_WaitEvent(&event) && (SDL_WINDOW_MOUSE_FOCUS & SDL_GetWindowFlags(window))) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_LEFT && checkInsideScreen(window, event.button.x, event.button.y)) {
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
					} else if (currCanvas.drawMode == 4) {
						erase(renderWindow, renderer, window, running);
					}
				}
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				cameraChange(renderWindow, event, renderer, window);
				bool changeMode = false;
				changeState(event, changeMode, currCanvas);
			}
		}
	}

	quit(window, currCanvas);

	return 0;
}