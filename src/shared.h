/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <PL/platform_console.h>
#include <PL/platform_filesystem.h>
#include <PL/platform_graphics.h>
#include <PL/platform_math.h>

#define DEBUG_BUILD

#define VC_LOG "debug"
#define VC_TITLE "Critter"
#define VC_VERSION "Pre-Alpha v0.1.0"

enum {
	VC_LOG_MSG,// generic message
	VC_LOG_DEB,// debug message (won't be displayed in shipped build)
	VC_LOG_WAR,// warning
	VC_LOG_ERR,// error (kills application)
};

#define PRINT( ... )       \
	printf( __VA_ARGS__ ); \
	plLogMessage( VC_LOG_MSG, __VA_ARGS__ )
#define PRINT_WARN( ... )  \
	printf( __VA_ARGS__ ); \
	plLogMessage( VC_LOG_WAR, __VA_ARGS__ )
#define PRINT_ERROR( ... )                   \
	printf( __VA_ARGS__ );                   \
	plLogMessage( VC_LOG_ERR, __VA_ARGS__ ); \
	exit( -1 )
#if defined( DEBUG_BUILD )
#define DPRINT( ... )      \
	printf( __VA_ARGS__ ); \
	plLogMessage( VC_LOG_DEB, __VA_ARGS__ )
#else
#define DPRINT( ... )
#endif

/*	Types	*/

typedef unsigned char byte;

/*	Engine	*/

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#ifdef __cplusplus

namespace engine {
	ALLEGRO_FONT *LoadFont( const char *path, unsigned int size );
	ALLEGRO_SAMPLE *LoadSample( const std::string &name );
	ALLEGRO_BITMAP *LoadImage( const char *path );
}// namespace engine

namespace vc {
	ALLEGRO_BITMAP *LoadImage( const std::string &path );
}

#endif

void DrawBitmap( ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h );
void DrawString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message );
void DrawShadowString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message );
void DrawCenteredString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message );
void DrawFilledRectangle( PLVector2 position, float w, float h, ALLEGRO_COLOR colour );
void DrawVerticalGradientRectangle( float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom );

enum InputMouseButton {
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,

	MAX_MOUSE_BUTTONS
};

struct EngineState {
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_TEXTLOG *log;

	ALLEGRO_MOUSE_STATE mouse_state;
	ALLEGRO_KEYBOARD_STATE keyboard_state;

	bool key_status[ ALLEGRO_KEY_MAX ];
	bool mouse_status[ MAX_MOUSE_BUTTONS ];// left, right, middle

	bool redraw;
	bool running;

	unsigned int window_width, window_height;
	int scalex, scaley, scalew, scaleh;

	double counter;

	char app_data[ PL_SYSTEM_MAX_PATH ];

	ALLEGRO_BITMAP *buffer{ nullptr };
};

extern EngineState engine_vars;

/*	Game	*/

void Game_Initialize();
void Game_Shutdown();

void GameDisplayFrame();
void Game_Tick();

void MouseEvent();
void Game_KeyboardEvent( int code, bool keyup );
