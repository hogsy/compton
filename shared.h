// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "platform_math.h"

#define DEBUG_BUILD

/*	Types	*/

typedef unsigned char byte;

/*	Engine	*/

typedef struct EngineVars {
	ALLEGRO_DISPLAY 		*display;
	ALLEGRO_EVENT_QUEUE 	*event_queue;
	ALLEGRO_TIMER			*timer;
	ALLEGRO_TEXTLOG			*log;
	ALLEGRO_MOUSE_STATE		mouse_state;

	bool redraw;
	bool running;

	unsigned int window_width, window_height;
	int scalex, scaley, scalew, scaleh;

	double counter;

	ALLEGRO_BITMAP *buffer;
} EngineVars;

extern EngineVars engine;

#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT	480

#define WINDOW_WIDTH	DISPLAY_WIDTH
#define WINDOW_HEIGHT	DISPLAY_HEIGHT

ALLEGRO_FONT *LoadFont(std::string path, unsigned int size);
ALLEGRO_BITMAP *LoadImage(std::string path);

void DrawBitmap(ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h);
void DrawString(const ALLEGRO_FONT *font, int x, int y,	ALLEGRO_COLOR colour, std::string message);
void DrawCenteredString(const ALLEGRO_FONT *font, int x, int y,	ALLEGRO_COLOR colour, std::string message);
void DrawFilledRectangle(PLVector2D position, float w, float h, ALLEGRO_COLOR colour);
void DrawVerticalGradientRectangle(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom);

/*	Game	*/

void InitializeGame();
void ShutdownGame();

void GameDisplayFrame();
void GameTimerFrame();

