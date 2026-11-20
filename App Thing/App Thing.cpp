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

	float round; // value is just a unit length

	SDL_FRect bounds;

	std::vector<SDL_FPoint> completedShape;
};

struct menuRect
{
	Rect rect;

	float offsetX;
	float offsetY;
};

void createRoundedRectangle(Rect& rectangle)
{
	std::vector<SDL_FPoint> temp;

	SDL_FPoint p1 = rectangle.point1;
	SDL_FPoint p2 = rectangle.point2;

	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(p1.x - p2.x) / 2, std::abs(p1.y - p2.y) / 2)) {
		rad = std::min(std::abs(p1.x - p2.x) / 2, std::abs(p1.y - p2.y) / 2);
	}
	
	SDL_FPoint corners[4] = {
		SDL_FPoint(std::max(p1.x, p2.x) - rad, std::max(p1.y, p2.y) - rad),
		SDL_FPoint(std::min(p1.x, p2.x) + rad, std::max(p1.y, p2.y) - rad),
		SDL_FPoint(std::min(p1.x, p2.x) + rad, std::min(p1.y, p2.y) + rad),
		SDL_FPoint(std::max(p1.x, p2.x) - rad, std::min(p1.y, p2.y) + rad)
	};

	for (int c = 0; c < 4; c++) {
		for (int i = 0; i < 90; i++) {
			float theta = (90 * c + i) * std::numbers::pi_v<float> / 180.0f;
			SDL_FPoint pt = SDL_FPoint(corners[c].x + (rad * std::cos(theta)), corners[c].y + (rad * std::sin(theta)));
			temp.push_back({pt});
		}
	}

	temp.push_back(SDL_FPoint(std::max(p1.x, p2.x), std::max(p1.y, p2.y) - rad));

	rectangle.completedShape = temp;
}

Rect createRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r)
{
	Rect temp;
	temp.point1 = p1;
	temp.point2 = p2;
	temp.c = c;
	temp.t = t;
	temp.round = r;

	createRoundedRectangle(temp);

	return temp;
}

menuRect createMenuRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r, float offsetX, float offsetY)
{
	menuRect temp;
	temp.rect = createRect(p1, p2, c, t, r);
	temp.offsetX = offsetX;
	temp.offsetY = offsetY;

	return temp;
}

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

	//Temp stuff
	std::vector<menuRect> menuTable;
};

bool checkRoundedCollision(float p1, float p2, Rect rectangle)
{
	SDL_FPoint pt1 = rectangle.point1;
	SDL_FPoint pt2 = rectangle.point2;
	
	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
		rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
	}

	float cx = std::max(std::min(pt1.x, pt2.x) + rad, std::min(p1, std::max(pt1.x, pt2.x) - rad));
	float cy = std::max(std::min(pt1.y, pt2.y) + rad, std::min(p2, std::max(pt1.y, pt2.y) - rad));

	std::cout << std::to_string(cx) + "\n";
	std::cout << std::to_string(cy) + "\n";
	return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
}

bool checkRoundedCollision(float p1, float p2, menuRect rect)
{
	Rect rectangle = rect.rect;

	SDL_FPoint pt1 = rectangle.point1;
	SDL_FPoint pt2 = rectangle.point2;
	
	float rad = rectangle.round;
	if (rectangle.round > std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2)) {
		rad = std::min(std::abs(pt1.x - pt2.x) / 2, std::abs(pt1.y - pt2.y) / 2);
	}

	float cx = std::max(std::min(pt1.x, pt2.x) + rad, std::min(p1, std::max(pt1.x, pt2.x) - rad));
	float cy = std::max(std::min(pt1.y, pt2.y) + rad, std::min(p2, std::max(pt1.y, pt2.y) - rad));

	std::cout << std::to_string(cx) + "\n";
	std::cout << std::to_string(cy) + "\n";
	return std::sqrt(std::pow(p1 - cx, 2) + std::pow(p2 - cy, 2)) <= rectangle.round;
}

void renderLines(SDL_Renderer* renderer, Canvas& currCanvas, const SDL_FRect& viewport)
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

void renderSpecificMenuShape(SDL_Renderer* renderer, menuRect rect)
{
	std::vector<SDL_FPoint> temp;
	Rect rectangle = rect.rect;
	SDL_SetRenderDrawColor(renderer, rectangle.c.r, rectangle.c.g, rectangle.c.b, rectangle.c.a);
	for (SDL_FPoint temptemp : rectangle.completedShape) {
		temptemp.x = (temptemp.x - (std::abs(rectangle.point1.x - rectangle.point2.x) / 2));
		temptemp.y = (temptemp.y - (std::abs(rectangle.point1.y - rectangle.point2.y) / 2));
		temp.push_back(temptemp);
	}
	SDL_RenderLines(renderer, rectangle.completedShape.data(), static_cast<int>(rectangle.completedShape.size()));
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

void menu(SDL_Renderer* renderer, std::vector<menuRect> menuBox)
{	
	for (const menuRect temp : menuBox) {
		renderSpecificMenuShape(renderer, temp);
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
	if (drawMode == 1 || drawMode == 2) {
		renderCurrLine(renderer, mouseX, mouseY, currCanvas);
	} else if (drawMode == 3) {
		renderCurrShape(renderer, mouseX, mouseY, currCanvas);
	}

	menu(renderer, currCanvas.menuTable);
	
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

		if (drawMode != 2) {
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

void createMenu(Canvas currCanvas)
{
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(0, 10), SDL_FPoint(300, 110), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
	currCanvas.menuTable.push_back(createMenuRect(SDL_FPoint(100, 60), SDL_FPoint(300, 300), SDL_Color(255, 0, 0, 255), 1, 20, 0, 0));
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
	currCanvas.currShape.c = SDL_Color(0, 255, 0, 255);
	
	createMenu(currCanvas);

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
					if (checkRoundedCollision(mouseX, mouseY, currCanvas.menuTable[0])) {
						std::cout << "done!";
					} else if (drawMode == 1) {
						if (modState & SDL_KMOD_CTRL) {
							endPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
							redraw = true;
						} else {
							std::cout << std::to_string(mouseX) + "|" + std::to_string(mouseY) + "\n";
							addPoints(mouseX, mouseY, currCanvas);
							redraw = true;
						}
					} else if (drawMode == 2) {
						freeDraw(currCanvas, renderer, window);
					} else if (drawMode == 3) {
						shapeFirstPoint(mouseX, mouseY, currCanvas);
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
				bool changeMode = false;
				changeState(event, drawMode, changeMode, currCanvas);
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