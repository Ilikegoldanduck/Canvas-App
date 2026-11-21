// CMakeProject1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cmath>
#include <numbers>
#include <variant>
#include <SDL3/SDL.h>

// TODO: Reference additional headers your program requires here.

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

struct Button
{
	Rect rect;
	std::string text;
};

struct guiWindow
{
	std::string name;
	float x, y; //position based on the screen (ignores camera movement)

	Rect body; //main body (if clicked and held, it follows the movement.)
	std::vector<Rect> shapes;
	std::vector<Button> buttons;
};

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
    int drawMode;

	//Temp stuff
	std::vector<menuRect> menuTable;
};

struct totalWindow
{
	Canvas currCanvas;

	guiWindow drawTab;

	std::vector<guiWindow> windows;
};