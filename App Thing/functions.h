#pragma once

#include "App Thing.h"

void renderCurrLine(SDL_Renderer* renderer, float p1, float p2, const Canvas currCanvas);
void renderCurrShape(SDL_Renderer* renderer, float p1, float p2, const Canvas currCanvas);
void shapeFirstPoint(float p1, float p2, Canvas& currCanvas);
void shapeSecondPoint(float p1, float p2, Canvas& currCanvas);
void menu(SDL_Renderer* renderer, const std::vector<menuRect> menuBox, SDL_Window* window);
void redrawScreen(SDL_Renderer* renderer, const totalWindow renderWindow, SDL_Window* window);
void addPoints(float p1, float p2, Canvas& currCanvas);
void endPoints(std::optional<float> p1, std::optional<float> p2, Canvas& currCanvas);
void shapeDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void freeDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void changeState(SDL_Event event, bool& changeMode, Canvas& currCanvas);
void createRoundedRectangle(Rect& rectangle);
Rect createRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r);
menuRect createMenuRect(SDL_FPoint p1, SDL_FPoint p2, SDL_Color c, float t, float r, float offsetX, float offsetY);
bool checkRoundedCollision(float p1, float p2, Rect rectangle);
bool checkMenuCollision(float p1, float p2, totalWindow& renderWindow, SDL_Window* window, SDL_Renderer* renderer);
void renderLines(SDL_Renderer* renderer, const Canvas& currCanvas, const SDL_FRect& viewport);
void renderSpecificMenuShape(SDL_Renderer* renderer, menuRect rect, SDL_Window* window);
void lineDraw(totalWindow& renderWindow, SDL_Renderer* renderer, SDL_Window* window, bool& running);
void cameraChange(totalWindow& renderWindow, SDL_Event event, SDL_Renderer* renderer, SDL_Window* window);
void createDrawWindow(guiWindow& drawWindow);
void moveDrawTabGuiWindow(SDL_Renderer* renderer, totalWindow& renderWindow, guiWindow& drawTab, SDL_Window* window, float x, float y);
void renderMenu(guiWindow guiWindow, SDL_Renderer* renderer, SDL_Window* window);
bool drawTab_checkGuiCollision(guiWindow guiWindow, SDL_Renderer* renderer, SDL_Window* window, totalWindow& renderWindow);
bool checkButtonCollision(float p1, float p2, const Rect& rectangle, const guiWindow guiWindow);
