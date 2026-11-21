#pragma once

#include "App Thing.h"
#include "functions.h"

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

void renderShapes(SDL_Renderer* renderer, Canvas& currCanvas, const SDL_FRect& viewport)
{
	std::vector<SDL_FPoint> transformed;
	for (const Rect rectangle : currCanvas.shapes)
	{
		if (!SDL_HasRectIntersectionFloat(&rectangle.bounds, &viewport))
		{
			continue;
		}
		SDL_SetRenderDrawColor(renderer, rectangle.c.r, rectangle.c.g, rectangle.c.b, rectangle.c.a);

		transformed.clear();
		transformed.reserve(rectangle.completedShape.size());

		for (const SDL_FPoint& p : rectangle.completedShape)
		{
			transformed.push_back({
				(p.x - currCanvas.c.x),
				(p.y - currCanvas.c.y)
			});
		}

		SDL_RenderLines(renderer, transformed.data(), static_cast<int>(rectangle.completedShape.size()));
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

void menu(SDL_Renderer* renderer, std::vector<menuRect> menuBox, SDL_Window* window)
{	
	for (const menuRect temp : menuBox) {
		renderSpecificMenuShape(renderer, temp, window);
	}
}

void redrawScreen(SDL_Renderer* renderer, totalWindow renderWindow, SDL_Window* window)
{
	Canvas& currCanvas = renderWindow.currCanvas;

	SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
	SDL_RenderClear(renderer);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect viewport = {currCanvas.c.x, currCanvas.c.y, static_cast<float>(width), static_cast<float>(height)};

	renderLines(renderer, currCanvas, viewport);
	if (currCanvas.drawMode == 1 || currCanvas.drawMode == 2) {
		renderCurrLine(renderer, mouseX, mouseY, currCanvas);
	} else if (currCanvas.drawMode == 3) {
		renderCurrShape(renderer, mouseX, mouseY, currCanvas);
	}

	for (const guiWindow menu : renderWindow.windows) {
		renderMenu(menu, renderer, window);
	}

	menu(renderer, currCanvas.menuTable, window);
	
	SDL_RenderPresent(renderer);
}

void addPoints(float p1, float p2, Canvas& currCanvas)
{
	currCanvas.currLine.points.push_back(SDL_FPoint(p1 + currCanvas.c.x, p2 + currCanvas.c.y));
}

void endPoints(std::optional<float> p1, std::optional<float> p2, Canvas& currCanvas)
{
	if (currCanvas.currLine.points.empty()) {
		return;
	}
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

void changeState(SDL_Event event, bool& changeMode, Canvas& currCanvas)
{
	if (event.key.key == SDLK_1) {
		currCanvas.drawMode = 1;
		changeMode = true;
	} else if (event.key.key == SDLK_2) {
		currCanvas.drawMode = 2;
		changeMode = true;
	} else if (event.key.key == SDLK_3) {
		currCanvas.drawMode = 3;
		changeMode = true;
	}
	if (changeMode) {
		if (!currCanvas.currLine.points.empty()) {
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}

void shapeDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running)
{
	bool done = false;

	while (! done)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
                running = false;
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					shapeSecondPoint(event.button.x, event.button.y, renderWindow.currCanvas);
					done = true;
					redrawScreen(renderer, renderWindow, window);
				}
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				redrawScreen(renderer, renderWindow, window);
			}
		}
	}
}

void freeDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running)
{
	bool done = false;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	Canvas& currCanvas = renderWindow.currCanvas;

	while (! done)
	{
		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
                running = false;
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					endPoints(event.button.x + currCanvas.c.x, event.button.y + currCanvas.c.y, currCanvas);
					done = true;	
					redrawScreen(renderer, renderWindow, window);
				}
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				addPoints(mouseX, mouseY, currCanvas);
				redrawScreen(renderer, renderWindow, window);
			}
		}

		if (renderWindow.currCanvas.drawMode != 2) {
			done = true;
			endPoints(std::nullopt, std::nullopt, currCanvas);
		}
	}
}

void lineDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running)
{
    bool done = false;

    float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	Canvas& currCanvas = renderWindow.currCanvas;
	
	addPoints(mouseX, mouseY, currCanvas);

    while (! done) {

        SDL_Event event;

		if (SDL_WaitEvent(&event)) {
            
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
                running = false;
			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				redrawScreen(renderer, renderWindow, window);
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_Keymod modState = SDL_GetModState();

                if (modState & SDL_KMOD_CTRL) {
                    endPoints(mouseX + currCanvas.c.x, mouseY + currCanvas.c.y, currCanvas);
					redrawScreen(renderer, renderWindow, window);
                    done = true;
                } else {
                    std::cout << std::to_string(mouseX) + "|" + std::to_string(mouseY) + "\n";
                    addPoints(mouseX, mouseY, currCanvas);
					redrawScreen(renderer, renderWindow, window);
                }
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
				cameraChange(renderWindow, event, renderer, window);
				bool changeMode = false;
				changeState(event, changeMode, currCanvas);
			}
        }
    }
}

void cameraChange(totalWindow& renderWindow, SDL_Event event, SDL_Renderer* renderer, SDL_Window* window)
{
	Canvas& currCanvas = renderWindow.currCanvas;
    if (event.key.key == SDLK_UP) {
        currCanvas.c.y = currCanvas.c.y - 10;
		redrawScreen(renderer, renderWindow, window);
    } else if (event.key.key == SDLK_DOWN) {
        currCanvas.c.y = currCanvas.c.y + 10;
		redrawScreen(renderer, renderWindow, window);
    } else if (event.key.key == SDLK_LEFT) {
        currCanvas.c.x = currCanvas.c.x - 10;
		redrawScreen(renderer, renderWindow, window);
    } else if (event.key.key == SDLK_RIGHT) {
        currCanvas.c.x = currCanvas.c.x + 10;
		redrawScreen(renderer, renderWindow, window);
    }
}