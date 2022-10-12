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

#define CT_PROFILER

#include "lisp.h"

// Core services
#include "engine/Compton.h"
#include "engine/Timer.h"
#include "engine/SpriteManager.h"
#include "engine/Serializer.h"
#include "engine/FileSystem.h"
#include "engine/Console.h"
#include "engine/GameMode.h"

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define ENABLE_SCALING

#define ENABLE_MOUSE 1

namespace ct
{
	class DSGameMode;
	class SpriteManager;
	class EntityManager;
	class BitmapFont;
	class Background;

	namespace math
	{
		struct Vector2 : public ISerializable
		{
			Vector2( int x, int y ) : x( x ), y( y ) {}
			Vector2() = default;

			void Deserialize( Serializer *read ) override
			{
				x = read->ReadI32();
				y = read->ReadI32();
			}

			void Serialize( Serializer *write ) override
			{
				write->WriteI32( x );
				write->WriteI32( y );
			}

			[[nodiscard]] Vector2 ToIso() const
			{
				return { ( x - y ), ( x + y ) / 2 };
			}
			[[nodiscard]] Vector2 ToCart() const
			{
				return { ( 2 * y + x ) / 2, ( 2 * y - x ) / 2 };
			}

			inline Vector2 operator-( const Vector2 &v ) const
			{
				return { x - v.x, y - v.y };
			}
			inline Vector2 operator+( const Vector2 &v ) const
			{
				return { x + v.x, y + v.y };
			}
			inline Vector2 operator/( const Vector2 &v ) const
			{
				return { x / v.x, y / v.y };
			}

			inline void operator-=( const Vector2 &v )
			{
				*this = ( v - *this );
			}
			inline void operator+=( const Vector2 &v )
			{
				*this = ( v + *this );
			}

			int x{ 0 }, y{ 0 };
		};
	}// namespace math

	namespace render
	{
		enum class FlipDirection
		{
			FLIP_NONE,
			FLIP_HORIZONTAL,
			FLIP_VERTICAL,
		};

		void ClearDisplay();
		void DrawPixel( int x, int y, const hei::Colour &colour );
		void DrawLine( int sx, int sy, int ex, int ey, const hei::Colour &colour );
		void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, ct::render::FlipDirection flipDirection = ct::render::FlipDirection::FLIP_NONE );
		void DrawBitmapRegion( const uint8_t *pixels, int x, int y, int w, int h, int dx, int dy, int dw, int dh, bool alphaTest = false );
		void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour );

		void SetScissor( int x, int y, int w, int h );

		bool IsVolumeVisible( int x, int y, int w, int h );

		inline void TransformToIso( int ox, int oy, int *dx, int *dy )
		{
			*dx = ( ox - oy );
			*dy = ( ox + oy ) / 2;
		}
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

		static IGameMode *GetGameMode();
		void SetGameMode( IGameMode *gameMode );

		void Precache();

		void InitializeDisplay();
		void InitializeEvents();
		void InitializeGame();

		[[noreturn]] void Loop();

		ALLEGRO_SAMPLE *CacheSample( const char *path );

		inline BitmapFont *GetDefaultFont()
		{
			return defaultBitmapFont_;
		}

		static void ShowMessageBox( const char *title, const char *message, bool error );

		void Shutdown();

		unsigned int GetNumOfTicks() const { return numTicks; }

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

		// Game state
		IGameMode *gameMode_{ nullptr };

		// Resources
		std::unordered_map< std::string, ALLEGRO_SAMPLE * > samples;// todo: make obsolete
		BitmapFont *defaultBitmapFont_{ nullptr };                  // todo: this should replace the above...

		bool redraw;

		bool debugFPS_{ false };
		bool debugProfiler_{ false };
		bool debugDrawStats{ false };

		int windowWidth, windowHeight;
		float scaleX, scaleY, scaleW, scaleH;

		unsigned int numTicks;

		static constexpr unsigned int MAX_FPS_READINGS = 64;
		double fps_[ MAX_FPS_READINGS ]{};

	public:
		inline unsigned int GetAverageFPS() const
		{
			double t = 0.0;
			for ( double fp : fps_ ) t += fp;
			return ( unsigned int ) ( t / MAX_FPS_READINGS );
		}

	private:
		char appDataPath[ PL_SYSTEM_MAX_PATH ];

	public:
		ALLEGRO_LOCKED_REGION *region_{ nullptr };

	private:
		ALLEGRO_BITMAP *screenBitmap_{ nullptr };

		///////////////////////////////////////////////////////////////////

		Console console_;
	};

	App *GetApp();

	extern SpriteManager *spriteManager;
}// namespace ct

#if defined( CT_PROFILER )
#	define START_MEASURE() ct::GetApp()->StartPerformanceTimer( __PRETTY_FUNCTION__ )
#	define END_MEASURE()   ct::GetApp()->EndPerformanceTimer( __PRETTY_FUNCTION__ )
#else
#	define START_MEASURE()
#	define END_MEASURE()
#endif
