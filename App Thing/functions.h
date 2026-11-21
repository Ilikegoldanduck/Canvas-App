#pragma once

#include "App Thing.h"

void renderCurrLine(SDL_Renderer* renderer, float p1, float p2, Canvas currCanvas);
void renderCurrShape(SDL_Renderer* renderer, float p1, float p2, Canvas currCanvas);
void shapeFirstPoint(float p1, float p2, Canvas& currCanvas);
void shapeSecondPoint(float p1, float p2, Canvas& currCanvas);
void menu(SDL_Renderer* renderer, std::vector<menuRect> menuBox, SDL_Window* window);
void redrawScreen(SDL_Renderer* renderer, totalWindow renderWindow, SDL_Window* window);
void addPoints(float p1, float p2, Canvas& currCanvas);
void endPoints(std::optional<float> p1, std::optional<float> p2, Canvas& currCanvas);
void shapeDraw(Canvas& currCanvas, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void freeDraw(Canvas& currCanvas, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void changeState(SDL_Event event, int& drawMode, bool& changeMode, Canvas currCanvas);
void createRoundedRectangle(Rect& rectangle);
Rect createRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r);
menuRect createMenuRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r, float offsetX, float offsetY);
bool checkRoundedCollision(float p1, float p2, Rect rectangle);
bool checkRoundedMenuCollision(float p1, float p2, guiWindow drawWindow, SDL_Window* window);
void renderLines(SDL_Renderer* renderer, Canvas& currCanvas, const SDL_FRect& viewport);
void renderSpecificMenuShape(SDL_Renderer* renderer, menuRect rect, SDL_Window* window);
void createMenu(Canvas& currCanvas);
void cameraChange(Canvas& currCanvas, SDL_Event event, bool& redraw);
void lineDraw(Canvas& currCanvas, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void cameraChange(Canvas& currCanvas, SDL_Event event, SDL_Renderer* renderer, SDL_Window* window);
void createDrawWindow(guiWindow& drawWindow);
void moveGuiWindow(SDL_Renderer* renderer, guiWindow& guiWindow, Canvas& currCanvas, SDL_Window* window, float x, float y);