// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

#ifdef __cplusplus
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#endif
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "platform_math.h"
#include "platform_log.h"
#include "platform_window.h"
#include "platform_filesystem.h"
#include "platform_graphics.h"

#define DEBUG_BUILD

#define VC_LOG      "debug"
#define VC_TITLE    "Virtual Critters"
#if defined(DEBUG_BUILD)
#   define VC_VERSION "PRE-ALPHA DEBUG"
#else
#   define VC_VERSION "PRE-ALPHA"
#endif

/*	Types	*/

typedef unsigned char byte;

/*	Engine	*/

#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT	480

#define WINDOW_WIDTH	DISPLAY_WIDTH
#define WINDOW_HEIGHT	DISPLAY_HEIGHT

namespace engine {
    ALLEGRO_FONT *LoadFont(const char *path, unsigned int size);
    ALLEGRO_BITMAP *LoadImage(const char *path);
}

void DrawBitmap(ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h);
void DrawString(const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message);
void DrawCenteredString(const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message);
void DrawFilledRectangle(PLVector2D position, float w, float h, ALLEGRO_COLOR colour);
void DrawVerticalGradientRectangle(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom);

typedef struct EngineVars {
    ALLEGRO_DISPLAY 		*display;
    ALLEGRO_EVENT_QUEUE 	*event_queue;
    ALLEGRO_TIMER			*timer;
    ALLEGRO_TEXTLOG			*log;

    ALLEGRO_MOUSE_STATE		mouse_state;
    ALLEGRO_KEYBOARD_STATE  keyboard_state;

    bool key_status[ALLEGRO_KEY_MAX];
    bool mouse_status[2]; // left, right, middle

    bool redraw;
    bool running;

    unsigned int window_width, window_height;
    int scalex, scaley, scalew, scaleh;

    double counter;

    ALLEGRO_BITMAP *buffer;
} EngineVars;

extern EngineVars engine_vars;

/*	Game	*/

void InitializeGame();
void ShutdownGame();

void GameDisplayFrame();
void GameTimerFrame();

void MouseEvent(void);
void KeyboardEvent(int code, bool keyup);

