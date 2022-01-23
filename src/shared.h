// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#include <plcore/pl_console.h>
#include <plcore/pl_filesystem.h>
#include <plcore/pl_package.h>
#include <plcore/pl_math.h>
#include <plcore/pl_timer.h>

#include <plgraphics/plg.h>

#define DEBUG_BUILD

// Core services
#include "engine/Compton.h"
#include "engine/Timer.h"
#include "engine/SpriteManager.h"
#include "engine/Serializer.h"
#include "engine/FileSystem.h"

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 200

enum InputMouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,

	MAX_MOUSE_BUTTONS
};

namespace vc
{
	class GameMode;
	class SpriteManager;
	class EntityManager;
	class BitmapFont;
	class Background;

	namespace render
	{
		enum class FlipDirection
		{
			FLIP_NONE,
			FLIP_HORIZONTAL,
			FLIP_VERTICAL,
		};

		void ClearDisplay( void );
		void DrawPixel( int x, int y, const hei::Colour &colour );
		void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection = vc::render::FlipDirection::FLIP_NONE );
		void DrawBitmapRegion( const uint8_t *pixels, int x, int y, int w, int h, int dx, int dy, int dw, int dh, bool alphaTest = false );
		void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour );
	}// namespace render

	// Common misc types

	struct RGBA8
	{
		uint8_t r, g, b, a;
	};

	// Main app class
	class App
	{
	public:
		App( int argc, char **argv );

		static constexpr unsigned int MAX_FPS = 60;

		static GameMode *GetGameMode();

		void PrecacheResources();

		void InitializeDisplay();
		void InitializeEvents();
		void InitializeGame();

		[[noreturn]] void Loop();

		ALLEGRO_SAMPLE *CacheSample( const char *path );

		inline BitmapFont *GetDefaultFont()
		{
			return defaultBitmapFont_;
		}

		void ShowMessageBox( const char *title, const char *message, bool error );

		void Shutdown();

		unsigned int GetNumOfTicks() { return numTicks; }

		inline void GetWindowSize( int *dW, int *dH ) const
		{
			*dW = windowWidth;
			*dH = windowHeight;
		}

		inline void GetScaledSize( int *dW, int *dH ) const
		{
			*dW = scaleW;
			*dH = scaleH;
		}

		void GetCursorPosition( int *dX, int *dY ) const;
		bool GetKeyState( int key ) const;
		bool GetMouseState( int *dX, int *dY, InputMouseButton button );

	private:
		void GrabCursor( bool state = true );
		bool debugMouse_{ false };

	public:
		inline const char *GetAppDataPath() const { return appDataPath; }

		//////////////////////////////////////////////////////
		// PROFILING

		void StartPerformanceTimer( const char *identifier );
		void EndPerformanceTimer( const char *identifier );

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
		std::unordered_map< std::string, ALLEGRO_SAMPLE * > samples;// todo: make obsolete
		BitmapFont *defaultBitmapFont_{ nullptr };                  // todo: this should replace the above...

		bool keyStatus[ ALLEGRO_KEY_MAX ];
		bool mouseStatus[ MAX_MOUSE_BUTTONS ];// left, right, middle

		bool redraw;
		bool running;

		int windowWidth, windowHeight;
		float scaleX, scaleY, scaleW, scaleH;

		double numTicks;

		static constexpr unsigned int MAX_FPS_READINGS = 64;
		double fps_[ MAX_FPS_READINGS ]{};

	public:
		unsigned int GetAverageFPS() const
		{
			double t = 0.0;
			for ( unsigned int i = 0; i < MAX_FPS_READINGS; ++i )
			{
				t += fps_[ i ];
			}
			return ( unsigned int ) ( t / MAX_FPS_READINGS );
		}

	private:
		char appDataPath[ PL_SYSTEM_MAX_PATH ];

	public:
		ALLEGRO_LOCKED_REGION *region_{ nullptr };

	private:
		ALLEGRO_BITMAP *screenBitmap_{ nullptr };
	};

	App *GetApp();

	extern SpriteManager *spriteManager;
}// namespace vc

#if defined( DEBUG_BUILD )
#	define START_MEASURE() vc::GetApp()->StartPerformanceTimer( __PRETTY_FUNCTION__ )
#	define END_MEASURE()   vc::GetApp()->EndPerformanceTimer( __PRETTY_FUNCTION__ )
#else
#	define START_MEASURE()
#	define END_MEASURE()
#endif

/*	Game	*/
