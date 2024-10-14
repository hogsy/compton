// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

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
#include "engine/console.h"

#define GAME_TYPE_SFC

//void DrawPixel( int x, int y, const hei::Colour &colour );
//void DrawBitmap( const uint8_t *pixels, int x, int y, int w, int h, const vc::ImageManager::Palette *palette, bool alphaTest = false );
//void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour );

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
	class EntityManager;
	class ImageManager;
	class BitmapFont;
	class Background;

	class App
	{
	public:
		App( int argc, char **argv );

		static GameMode *GetGameMode();

		void PrecacheResources();

		void InitializeDisplay();
		void InitializeEvents();
		void InitializeGame();

		bool IsRunning();
		void Loop();

		ALLEGRO_SAMPLE *CacheSample( const char *path );
		ALLEGRO_BITMAP *CacheImage( const char *path );

		inline BitmapFont *GetDefaultFont()
		{
			return defaultBitmapFont_;
		}

		void ShowMessageBox( const char *title, const char *message, bool error );

		void Shutdown();

		unsigned int GetNumOfTicks() { return numTicks; }

		inline int GetDrawWidth() const { return drawWidth; }
		inline int GetDrawHeight() const { return drawHeight; }

		void GetCursorPosition( int *dX, int *dY ) const;
		bool GetKeyState( int key ) const;
		bool GetMouseState( int *dX, int *dY, InputMouseButton button );

	private:
		void GrabCursor( bool state = true );

	public:
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

		std::map< std::string, engine::Timer > performanceTimers;

		ALLEGRO_DISPLAY     *alDisplay;
		ALLEGRO_EVENT_QUEUE *alEventQueue;
		ALLEGRO_TIMER       *alTimer;
		ALLEGRO_TEXTLOG     *alLog;

		ALLEGRO_MOUSE_STATE    mouseState;
		ALLEGRO_KEYBOARD_STATE keyboardState;

		// Game state
		GameMode *gameMode{ nullptr };

		// Resources
		std::unordered_map< std::string, ALLEGRO_SAMPLE * > samples;                      // todo: make obsolete
		BitmapFont                                         *defaultBitmapFont_{ nullptr };// todo: this should replace the above...

		bool keyStatus[ ALLEGRO_KEY_MAX ];
		bool mouseStatus[ MAX_MOUSE_BUTTONS ];// left, right, middle

		bool redraw;

		bool debugFPS_{};
		bool debugProfiler_{};
		bool debugDrawStats{};

		bool running;

		int                  displayWidth, displayHeight;
		int                  drawWidth, drawHeight;
		static constexpr int scale = 4;

		double numTicks{};

		static constexpr unsigned int MAX_FPS_READINGS = 64;
		double                        fps_[ MAX_FPS_READINGS ]{};

	public:
		inline unsigned int GetAverageFPS() const
		{
			double t = 0.0;
			for ( double fp : fps_ ) t += fp;
			return ( unsigned int ) ( t / MAX_FPS_READINGS );
		}

	private:
		char appDataPath[ PL_SYSTEM_MAX_PATH ];

		// Sub-Systems
		ImageManager *imageManager{ nullptr };

	public:
		ALLEGRO_LOCKED_REGION *region_{ nullptr };

	private:
		ALLEGRO_BITMAP *screenBitmap_{ nullptr };

		engine::Console console{};
	};

	App *GetApp();
}// namespace vc

#if defined( DEBUG_BUILD )
#	define START_MEASURE() vc::GetApp()->StartPerformanceTimer( __PRETTY_FUNCTION__ )
#	define END_MEASURE()   vc::GetApp()->EndPerformanceTimer( __PRETTY_FUNCTION__ )
#else
#	define START_MEASURE()
#	define END_MEASURE()
#endif

/*	Game	*/
