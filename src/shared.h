/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
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

#include <plcore/pl_console.h>
#include <plcore/pl_filesystem.h>
#include <plcore/pl_package.h>
#include <plcore/pl_math.h>

#include <plgraphics/plg.h>

#define DEBUG_BUILD

#include "engine/Compton.h"
#include "engine/Timer.h"
#include "engine/ImageManager.h"

#define DISPLAY_WIDTH   640
#define DISPLAY_HEIGHT  480

#define GAME_TYPE_SFC

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
	class EntityManager;
	class BitmapFont;
	class Background;

	class App {
	public:
		App( int argc, char **argv );

		static GameMode *GetGameMode();

		void PrecacheResources();

		void InitializeDisplay();
		void InitializeEvents();
		void InitializeGame();

		bool IsRunning();
		void Loop();

		ALLEGRO_FONT *CacheFont( const char *path, unsigned int size );
		ALLEGRO_SAMPLE *CacheSample( const char *path );
		ALLEGRO_BITMAP *CacheImage( const char *path );

		inline BitmapFont *GetDefaultFont() {
			return defaultBitmapFont_;
		}

		void ShowMessageBox( const char *title, const char *message, bool error );

		void Shutdown();

		unsigned int GetNumOfTicks() { return numTicks; }

		inline void GetWindowSize( int *dW, int *dH ) const {
			*dW = windowWidth;
			*dH = windowHeight;
		}

		inline void GetScaledSize( int *dW, int *dH ) const {
			*dW = scaleW;
			*dH = scaleH;
		}

		void GetCursorPosition( int *dX, int *dY ) const;
		bool GetKeyState( int key ) const;
		bool GetMouseState( int *dX, int *dY, InputMouseButton button );

		inline const char *GetAppDataPath() const { return appDataPath; }

		//////////////////////////////////////////////////////
		// PROFILING

		void StartPerformanceTimer( const char *identifier );
		void EndPerformanceTimer( const char *identifier );

		inline ImageManager *GetImageManager() { return imageManager; }

	protected:
	private:
		~App();

		void Draw();
		void Tick();

		std::map< std::string, Timer > performanceTimers;

		ALLEGRO_DISPLAY *alDisplay;
		ALLEGRO_EVENT_QUEUE *alEventQueue;
		ALLEGRO_TIMER *alTimer;
		ALLEGRO_TEXTLOG *alLog;

		ALLEGRO_MOUSE_STATE mouseState;
		ALLEGRO_KEYBOARD_STATE keyboardState;

		// Game state
		GameMode *gameMode{ nullptr };

		// Resources
		std::unordered_map< std::string, ALLEGRO_BITMAP * > bitmaps; // todo: make obsolete
		std::unordered_map< std::string, ALLEGRO_SAMPLE * > samples; // todo: make obsolete
		std::unordered_map< std::string, ALLEGRO_FONT * > fonts; // todo: make obsolete
		ALLEGRO_FONT *defaultFont; // todo: make obsolete
		BitmapFont *defaultBitmapFont_{ nullptr }; // todo: this should replace the above...
		std::map< std::string, BitmapFont * > bitmapFonts_; // todo: this should replace the above...

		bool keyStatus[ ALLEGRO_KEY_MAX ];
		bool mouseStatus[ MAX_MOUSE_BUTTONS ];// left, right, middle

		bool redraw;
		bool running;

		unsigned int windowWidth, windowHeight;
		int scaleX, scaleY, scaleW, scaleH;

		double numTicks;

		char appDataPath[ PL_SYSTEM_MAX_PATH ];

		// Sub-Systems
		ImageManager *imageManager{ nullptr };

	public:
		ALLEGRO_BITMAP *buffer{ nullptr };
	};

	App *GetApp();
}// namespace vc

#if defined( DEBUG_BUILD )
#   define START_MEASURE() vc::GetApp()->StartPerformanceTimer( __PRETTY_FUNCTION__ )
#   define END_MEASURE()   vc::GetApp()->EndPerformanceTimer( __PRETTY_FUNCTION__ )
#else
#   define START_MEASURE()
#   define END_MEASURE()
#endif

/*	Game	*/
