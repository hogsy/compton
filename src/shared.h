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
#include <PL/pl_graphics.h>
#include <PL/platform_package.h>
#include <PL/platform_math.h>

#define DEBUG_BUILD

#include "engine/SimGame.h"

#define DISPLAY_WIDTH   320
#define DISPLAY_HEIGHT  240

#ifdef DEBUG_BUILD
#   define WINDOW_TITLE "SimGame [DEBUG]"
#else
#   define WINDOW_TITLE "SimGame"
#endif
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   960

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

namespace vc {
	class GameMode;
	class App {
	public:
		App( int argc, char **argv );

		void InitializeDisplay();
		void InitializeEvents();
		void InitializeGame();

		bool IsRunning();
		void Loop();

		ALLEGRO_FONT *CacheFont( const char *path, unsigned int size );
		ALLEGRO_SAMPLE *CacheSample( const char *path );
		ALLEGRO_BITMAP *CacheImage( const char *path );

		void ShowMessageBox( const char *title, const char *message, bool error );

		void Shutdown();

		unsigned int GetNumOfTicks() { return numTicks; }
	protected:
	private:
		~App();

		void Draw();

		void Tick();

		ALLEGRO_DISPLAY *alDisplay;
		ALLEGRO_EVENT_QUEUE *alEventQueue;
		ALLEGRO_TIMER *alTimer;
		ALLEGRO_TEXTLOG *alLog;

		ALLEGRO_MOUSE_STATE mouseState;
		ALLEGRO_KEYBOARD_STATE keyboardState;

		// Game state
		GameMode *gameMode{ nullptr };

		// Resources
		std::unordered_map< std::string, ALLEGRO_BITMAP * > bitmaps;
		std::unordered_map< std::string, ALLEGRO_SAMPLE * > samples;
		std::unordered_map< std::string, ALLEGRO_FONT * > fonts;

		bool keyStatus[ ALLEGRO_KEY_MAX ];
		bool mouseStatus[ MAX_MOUSE_BUTTONS ];// left, right, middle

		bool redraw;
		bool running;

		unsigned int windowWidth, windowHeight;
		int scaleX, scaleY, scaleW, scaleH;

		double numTicks;

		char appDataPath[ PL_SYSTEM_MAX_PATH ];

		ALLEGRO_BITMAP *buffer{ nullptr };
	};

	App *GetApp();
}// namespace vc

/*	Game	*/
