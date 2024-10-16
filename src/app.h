
#pragma once

#include <cstdint>
#include <cstdio>

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>

#include "plcore/pl_console.h"
#include "plcore/pl_math.h"

#include "timer.h"
#include "console.h"
#include "input/input.h"

extern int VC_LOG_MSG;// generic message
extern int VC_LOG_DEB;// debug message (won't be displayed in shipped build)
extern int VC_LOG_WAR;// warning
extern int VC_LOG_ERR;// error (kills application)

#define VC_LOG   "debug"
#define VC_TITLE "BitCritters"

#define GAME_TYPE_SFC

#ifdef DEBUG_BUILD
#	define WINDOW_TITLE VC_TITLE " [DEBUG]"
#else
#	define WINDOW_TITLE VC_TITLE
#endif

#define Print( ... ) \
	PlLogMessage( VC_LOG_MSG, __VA_ARGS__ )
#define Warning( ... ) \
	PlLogMessage( VC_LOG_WAR, __VA_ARGS__ )
#define Error( ... )                             \
	{                                            \
		PlLogMessage( VC_LOG_ERR, __VA_ARGS__ ); \
		abort();                                 \
	}
#if defined( DEBUG_BUILD )
#	define DebugMsg( ... ) \
		PlLogMessage( VC_LOG_DEB, __VA_ARGS__ )
#else
#	define DebugMsg( ... )
#endif

#define IMPLEMENT_SUPER( CLASS ) typedef CLASS SuperClass;

class GameMode;
class EntityManager;
class ImageManager;
class Background;

enum InputMouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,

	MAX_MOUSE_BUTTONS
};

class BitmapFont;

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

	inline BitmapFont *get_default_font()
	{
		return defaultBitmapFont_;
	}

	void ShowMessageBox( const char *title, const char *message, bool error );

	void Shutdown();

	unsigned int GetNumOfTicks() { return numTicks; }

	inline int GetDrawWidth() const { return drawWidth; }
	inline int get_draw_height() const { return drawHeight; }

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

	inline input::InputManager *GetInputManager() { return inputManager; }
	inline ImageManager        *GetImageManager() { return imageManager; }

protected:
private:
	~App();

	void Draw();
	void Tick();

	std::map< std::string, Timer > performanceTimers;

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
	ImageManager        *imageManager{};
	input::InputManager *inputManager{};

public:
	ALLEGRO_LOCKED_REGION *region_{ nullptr };

private:
	ALLEGRO_BITMAP *screenBitmap_{ nullptr };

	Console console{};
};

App *GetApp();

#if !defined( NDEBUG )
#	define START_MEASURE() GetApp()->StartPerformanceTimer( __PRETTY_FUNCTION__ )
#	define END_MEASURE()   GetApp()->EndPerformanceTimer( __PRETTY_FUNCTION__ )
#else
#	define START_MEASURE()
#	define END_MEASURE()
#endif
