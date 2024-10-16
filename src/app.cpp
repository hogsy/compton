// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include <stdexcept>

#include <plcore/pl_timer.h>
#include "plcore/pl_package.h"
#include "plcore/pl_image.h"

#include <allegro5/allegro_acodec.h>

#include "lisp.h"

#include "app.h"
#include "game_mode.h"

#include "input/input.h"

#include "render/render_bitmap_font.h"
#include "render/render.h"

////////////////////////////////
// App Class

static App *appInstance;
App        *GetApp()
{
	return appInstance;
}

extern ALLEGRO_FILE_INTERFACE g_fsIOInterface;

///////////////////////////////////////////////////////////////////
// Memory Management

// Override C++ new/delete operators, so we can track memory usage
void *operator new( size_t size ) { return PlMAllocA( size ); }
void *operator new[]( size_t size ) { return PlMAllocA( size ); }
void  operator delete( void *p ) noexcept { PlFree( p ); }
void  operator delete[]( void *p ) noexcept { PlFree( p ); }
// And below is a wrapper for Allegro, so we can do the same there
static void *AlMAlloc( size_t n, int, const char *, const char * ) { return PlMAllocA( n ); }
static void  AlFree( void *p, int, const char *, const char  *) { PlFree( p ); }
static void *AlReAlloc( void *p, size_t n, int, const char *, const char * ) { return PlReAllocA( p, n ); }
static void *AlCAlloc( size_t c, size_t n, int, const char *, const char * ) { return PlCAllocA( c, n ); }

/**
 * Callback, to catch any fail within hei.
 */
static void MemoryAbortCallback( size_t failSize )
{
	Error( "Failed to alloc of %lu bytes!\n", failSize );
}

///////////////////////////////////////////////////////////////////

int VC_LOG_MSG;// generic message
int VC_LOG_DEB;// debug message (won't be displayed in shipped build)
int VC_LOG_WAR;// warning
int VC_LOG_ERR;// error (kills application)

static constexpr int DISPLAY_WIDTH  = 640;
static constexpr int DISPLAY_HEIGHT = 360;

App::App( int argc, char **argv )
{
	// Initialize the platform library

	pl_memory_abort_cb = MemoryAbortCallback;

	if ( PlInitialize( argc, argv ) != PL_RESULT_SUCCESS )
	{
		std::string msg = "Failed to initialize Hei: " + std::string( PlGetError() ) + "\n";
		throw std::runtime_error( msg );
	}

	if ( PlInitializeSubSystems( PL_SUBSYSTEM_IO ) != PL_RESULT_SUCCESS )
	{
		std::string msg = "Failed to initialize Hei: " + std::string( PlGetError() ) + "\n";
		throw std::runtime_error( msg );
	}

	PlGetApplicationDataDirectory( VC_TITLE, appDataPath, sizeof( appDataPath ) );
	PlCreateDirectory( appDataPath );

	std::string logDir = std::string( appDataPath ) + "/" + VC_LOG;
	PlSetupLogOutput( logDir.c_str() );

	VC_LOG_ERR = PlAddLogLevel( "error", { 255, 0, 0 }, true );
	VC_LOG_WAR = PlAddLogLevel( "warning", { 255, 255, 0 }, true );
	VC_LOG_MSG = PlAddLogLevel( "info", { 255, 255, 255 }, true );
	VC_LOG_DEB = PlAddLogLevel( "debug", { 0, 0, 255 }, true );

	Print( VC_TITLE " (build " GIT_COMMIT_COUNT " [" GIT_BRANCH ":" GIT_COMMIT_HASH "], compiled " __DATE__ ")\n" );

	PlRegisterStandardPackageLoaders( PL_PACKAGE_LOAD_FORMAT_ZIP );
	PlRegisterStandardImageLoaders( PL_IMAGE_FILEFORMAT_ALL );

	PlMountLocalLocation( appDataPath );
	PlMountLocalLocation( "./" );

	for ( unsigned int i = 0; i < 100; ++i )
	{
		char packageName[ 64 ];
		snprintf( packageName, sizeof( packageName ), "data%d.zip", i );
		if ( PlMountLocation( packageName ) == nullptr )
		{
			break;
		}
	}

	// And now initialize Allegro

	uint32_t version = al_get_allegro_version();
	uint32_t major   = version >> 24;
	uint32_t minor   = ( version >> 16 ) & 255;
	Print( "Initializing Allegro %d.%d\n", major, minor );
	if ( !al_init() )
	{
		Error( "Failed to initialize Allegro library!\n" );
	}

	static ALLEGRO_MEMORY_INTERFACE memoryInterface = {
	        AlMAlloc,
	        AlFree,
	        AlReAlloc,
	        AlCAlloc };
	al_set_memory_interface( &memoryInterface );

	if ( !al_install_mouse() )
	{
		Error( "Failed to install mouse through Allegro!\n" );
	}

#if !defined( EDITOR )
	if ( !al_install_audio() )
	{
		Error( "Failed to install audio through Allegro!\n" );
	}

	if ( !al_init_acodec_addon() )
	{
		Error( "Failed to install audio codecs through Allegro!\n" );
	}

	al_reserve_samples( 512 );
#endif

	al_init_native_dialog_addon();

	al_set_new_file_interface( &g_fsIOInterface );
	al_register_bitmap_loader( ".png", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".bmp", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".tga", ImageBitmap_LoadGeneric );

#if 0
	ALLEGRO_PATH *path = al_get_standard_path( ALLEGRO_RESOURCES_PATH );
	al_change_directory( al_path_cstr( path, '/' ) );
	al_destroy_path( path );
#endif

	// Doing this to ensure that rand is truly random
	// otherwise, for example, clouds will always spawn
	// in the same places every time.
	srand( ( unsigned ) time( nullptr ) );

	inputManager = new input::InputManager();
	imageManager = new ImageManager( argc, argv );

	running = true;

	PlRegisterConsoleVariable( "showfps", "Display the current average FPS.", "true", PL_VAR_BOOL, &debugFPS_, nullptr, false );
	PlRegisterConsoleVariable( "showprof", "Display profiler results.", "1", PL_VAR_BOOL, &debugProfiler_, nullptr, false );
	PlRegisterConsoleVariable( "showstat", "Display draw stats.", "1", PL_VAR_BOOL, &debugDrawStats, nullptr, false );
}

App::~App() = default;

bool App::IsRunning()
{
	// We're always running!
	return true;
}

void App::Loop()
{
	Tick();
	Draw();

	// Now clear all the performance timers
	performanceTimers.clear();
}

ALLEGRO_SAMPLE *App::CacheSample( const char *path )
{
#if !defined( EDITOR )
	auto i = samples.find( path );
	if ( i != samples.end() )
	{
		return i->second;
	}

	Print( "Caching sample, \"%s\"\n", path );

	ALLEGRO_SAMPLE *sample = al_load_sample( path );
	if ( sample == nullptr )
	{
		Error( "Failed to load sample, \"%s\"!\n", path );
	}

	samples.emplace( path, sample );

	return sample;
#else
	return nullptr;
#endif
}

void App::ShowMessageBox( const char *title, const char *message, bool error )
{
	al_show_native_message_box(
	        nullptr,
	        VC_TITLE,
	        title,
	        message,
	        nullptr,
	        error ? ALLEGRO_MESSAGEBOX_ERROR : ALLEGRO_MESSAGEBOX_WARN );
}

void App::Shutdown()
{
	Lisp::Uninit();

	delete gameMode;
	delete imageManager;

	if ( screenBitmap_ != nullptr )
	{
		al_destroy_bitmap( screenBitmap_ );
		screenBitmap_ = nullptr;
	}

	if ( alDisplay != nullptr )
	{
		al_destroy_display( alDisplay );
		alDisplay = nullptr;
	}

	if ( alEventQueue != nullptr )
	{
		al_destroy_event_queue( alEventQueue );
		alEventQueue = nullptr;
	}

	if ( alTimer != nullptr )
	{
		al_destroy_timer( alTimer );
		alTimer = nullptr;
	}

	exit( EXIT_SUCCESS );
}

// Display

void App::InitializeDisplay()
{
	Print( "Initializing display...\n" );

	displayWidth  = DISPLAY_WIDTH;
	displayHeight = DISPLAY_HEIGHT;

	al_set_new_display_flags( ALLEGRO_RESIZABLE );
	alDisplay = al_create_display( displayWidth, displayHeight );
	if ( alDisplay == nullptr )
	{
		Error( "Failed to initialize display!\n" );
	}

	// Get the actual width and height
	displayWidth  = al_get_display_width( alDisplay );
	displayHeight = al_get_display_height( alDisplay );

	al_set_window_title( alDisplay, WINDOW_TITLE );

	// Check to see how much we need to scale the buffer.
	al_set_new_bitmap_flags( ALLEGRO_MEMORY_BITMAP );
	al_set_new_bitmap_format( ALLEGRO_PIXEL_FORMAT_RGB_888 );
	screenBitmap_ = al_create_bitmap( DISPLAY_WIDTH, DISPLAY_HEIGHT );
	if ( screenBitmap_ == nullptr )
	{
		Error( "Failed to create screen buffer: %u\n", al_get_errno() );
	}

	drawWidth  = DISPLAY_WIDTH;
	drawHeight = DISPLAY_HEIGHT;

	al_inhibit_screensaver( true );

	redraw = true;
}

void App::Draw()
{
	if ( !redraw )
	{
		return;
	}

	START_MEASURE();

	// Now draw everything we want

	drawWidth  = displayWidth / scale;
	drawHeight = displayHeight / scale;

	if ( al_get_bitmap_width( screenBitmap_ ) != drawWidth || al_get_bitmap_height( screenBitmap_ ) != drawHeight )
	{
		al_destroy_bitmap( screenBitmap_ );

		al_set_new_bitmap_flags( ALLEGRO_MEMORY_BITMAP );
		al_set_new_bitmap_format( ALLEGRO_PIXEL_FORMAT_RGB_888 );
		screenBitmap_ = al_create_bitmap( drawWidth, drawHeight );
		if ( screenBitmap_ == nullptr )
		{
			Error( "Failed to create screen buffer: %u\n", al_get_errno() );
		}

		al_set_clipping_rectangle( 0, 0, displayWidth, displayHeight );
	}

	// Setup the target buffer
	al_set_target_bitmap( screenBitmap_ );

	region_ = al_lock_bitmap( screenBitmap_, al_get_bitmap_format( screenBitmap_ ), ALLEGRO_LOCK_READWRITE );

	render::BeginDraw();

	static double       oldTime;
	double              newTime = PlGetCurrentSeconds();
	static unsigned int ci      = 0;
	fps_[ ci++ ]                = 1.0 / ( newTime - oldTime );
	if ( ci >= MAX_FPS_READINGS )
	{
		ci = 0;
	}
	oldTime = newTime;

	render::SetScissor( 0, 0, drawWidth, drawHeight );
	render::ClearDisplay();

	gameMode->Draw();

	console.Draw();

	render::EndDraw();

	END_MEASURE();

	// Draw our debug data
	BitmapFont *font = get_default_font();
	if ( font != nullptr )
	{
		char buf[ 256 ];
		int  x = 8, y = 8;

		if ( debugFPS_ )
		{
			unsigned int fps = GetAverageFPS();

			snprintf( buf, sizeof( buf ), "FPS = %u\n", fps );
			hei::Colour colour;
			if ( fps <= 10 )
			{
				colour = hei::Colour( 255, 0, 0 );
			}
			else if ( fps <= 20 )
			{
				colour = hei::Colour( 255, 255, 0 );
			}
			else
			{
				colour = hei::Colour( 0, 255, 0 );
			}

			font->DrawString( &x, &y, buf, colour );
		}

		if ( debugProfiler_ )
		{
			for ( auto const &i : performanceTimers )
			{
				snprintf( buf, sizeof( buf ), "%s: %f\n", i.first.c_str(), i.second.GetTimeTaken() );
				font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 50 ) );
			}
		}

		if ( debugDrawStats )
		{
			for ( unsigned int i = 0; i < render::DrawStats::Type::MAX_DRAW_STATS_TYPES; ++i )
			{
				snprintf( buf, sizeof( buf ), "%s: %d\n",
				          render::DrawStats::GetDescription( ( render::DrawStats::Type ) i ),
				          render::drawStats.stats[ i ] );
				font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 50 ) );
			}
		}
	}

	al_unlock_bitmap( screenBitmap_ );

	double ba = ( double ) drawWidth / drawHeight;
	double da = ( double ) displayWidth / displayHeight;

	int nw, nh;
	if ( da > ba )
	{
		nh = displayHeight;
		nw = displayHeight * ba;
	}
	else
	{
		nw = displayWidth;
		nh = displayWidth / ba;
	}

	int ox = ( displayWidth - nw ) / scale;
	int oy = ( displayHeight - nh ) / scale;

	// And finally, handle the scaling
	al_set_target_backbuffer( alDisplay );
	al_draw_scaled_bitmap(
	        screenBitmap_,
	        0, 0,
	        drawWidth, drawHeight,
	        ox, oy,
	        nw, nh,
	        0 );

	al_flip_display();

	redraw = false;
}

// Events

void App::InitializeEvents()
{
	Print( "Initialize events...\n" );

	if ( ( alTimer = al_create_timer( 1.0 / 30 ) ) == nullptr )
	{
		Error( "Failed to initialize timer!\n" );
	}

	if ( ( alEventQueue = al_create_event_queue() ) == nullptr )
	{
		Error( "Failed to initialize events!\n" );
	}

	al_install_mouse();
	al_install_keyboard();

	GrabCursor();

	al_register_event_source( alEventQueue, al_get_display_event_source( alDisplay ) );
	al_register_event_source( alEventQueue, al_get_timer_event_source( alTimer ) );
	al_register_event_source( alEventQueue, al_get_keyboard_event_source() );
	al_register_event_source( alEventQueue, al_get_mouse_event_source() );

	al_start_timer( alTimer );

	// Clear out key states
	memset( keyStatus, 0, sizeof( bool ) * ALLEGRO_KEY_MAX );
}

void App::InitializeGame()
{
	Lisp::Init();

	gameMode = GetGameModeInterface();
}

void App::Tick()
{
	START_MEASURE();

	ALLEGRO_EVENT event{};
	al_wait_for_event( alEventQueue, &event );

	al_get_mouse_state( &mouseState );
	al_get_keyboard_state( &keyboardState );

	mouseStatus[ MOUSE_BUTTON_LEFT ]   = ( mouseState.buttons & 1 ) != 0;
	mouseStatus[ MOUSE_BUTTON_RIGHT ]  = ( mouseState.buttons & 2 ) != 0;
	mouseStatus[ MOUSE_BUTTON_MIDDLE ] = ( mouseState.buttons & 3 ) != 0;

	switch ( event.type )
	{
		default: break;

		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
#if defined( NDEBUG )
			al_hide_mouse_cursor( alDisplay );
#endif
			break;
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			al_show_mouse_cursor( alDisplay );
			break;

		case ALLEGRO_EVENT_DISPLAY_DISCONNECTED:
		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			GrabCursor( false );
			break;
		case ALLEGRO_EVENT_DISPLAY_CONNECTED:
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			GrabCursor( true );
			break;

		case ALLEGRO_EVENT_TIMER:
			numTicks++;
			if ( gameMode != nullptr )
			{
				gameMode->Tick();
			}
			redraw = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			running = false;
			break;

		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			al_acknowledge_resize( alDisplay );
			displayWidth  = event.display.width;
			displayHeight = event.display.height;
			break;

		case ALLEGRO_EVENT_MOUSE_AXES:
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			if ( console.IsOpen() && ( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) )
			{
				break;
			}

			InputMouseButton button = MOUSE_BUTTON_LEFT;
			if ( event.mouse.button & 1 )
			{
				button = MOUSE_BUTTON_LEFT;
			}
			else if ( event.mouse.button & 2 )
			{
				button = MOUSE_BUTTON_RIGHT;
			}
			else if ( event.mouse.button & 3 )
			{
				button = MOUSE_BUTTON_MIDDLE;
			}

			int aX = event.mouse.x * drawWidth / displayWidth;
			int aY = event.mouse.y * drawHeight / displayHeight;
			inputManager->HandleMouseEvent( aX, aY, event.mouse.dz, button, ( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) );
			break;
		}

		case ALLEGRO_EVENT_KEY_CHAR:
		{
			if ( !console.IsOpen() )
			{
				break;
			}

			console.PushCharacter( event.keyboard.unichar );
			break;
		}

		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_KEY_UP:
		{
			printf( "key is %s\n", ( event.type == ALLEGRO_EVENT_KEY_DOWN ) ? "down" : "up" );

			if ( console.IsOpen() )
			{
				if ( event.type != ALLEGRO_EVENT_KEY_DOWN )
				{
					break;
				}

				if ( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
				{
					console.Close();
				}
				else if ( event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_DOWN )
				{
					console.ScrollHistory( event.keyboard.keycode == ALLEGRO_KEY_UP );
				}

				break;
			}

			if ( event.type == ALLEGRO_EVENT_KEY_DOWN && ( event.keyboard.keycode == ALLEGRO_KEY_TILDE ||
			                                               event.keyboard.keycode == ALLEGRO_KEY_ENTER ) )
			{
				console.Open();
				break;
			}

			keyStatus[ event.keyboard.keycode ] = ( event.type != ALLEGRO_EVENT_KEY_UP );
			printf( "status is %s\n", keyStatus[ event.keyboard.keycode ] ? "true" : "false" );

			inputManager->HandleKeyboardEvent( event.keyboard.keycode, ( event.type == ALLEGRO_EVENT_KEY_UP ) );
			break;
		}
	}

	if ( !al_is_event_queue_empty( alEventQueue ) )
	{
		redraw = false;
	}

	END_MEASURE();
}

/**
 * Returns cursor position, accounting for scaled size.
 */
void App::GetCursorPosition( int *dX, int *dY ) const
{
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state( &state );

	*dX = state.x * drawWidth / displayWidth;
	*dY = state.y * drawHeight / displayHeight;
}

bool App::GetKeyState( int key ) const
{
	assert( key > 0 && key < ALLEGRO_KEY_MAX );
	return keyStatus[ key ];
}

bool App::GetMouseState( int *dX, int *dY, InputMouseButton button )
{
	if ( dX != nullptr && dY != nullptr )
	{
		GetCursorPosition( dX, dY );
	}

	return mouseStatus[ button ];
}

void App::GrabCursor( bool status )
{
	if ( status )
	{
#if defined( NDEBUG )
		al_grab_mouse( alDisplay );
		al_hide_mouse_cursor( alDisplay );
#endif
		return;
	}

	al_ungrab_mouse();
	al_show_mouse_cursor( alDisplay );
}

//////////////////////////////////////////////////////
// PROFILING

void App::StartPerformanceTimer( const char *identifier )
{
	performanceTimers.insert( std::pair< std::string, Timer >( identifier, Timer() ) );
}

void App::EndPerformanceTimer( const char *identifier )
{
	auto i = performanceTimers.find( identifier );
	if ( i == performanceTimers.end() )
	{
		Warning( "Attempted to end an invalid timer, \"%s\"!\n", identifier );
		return;
	}

	i->second.End();
}

//////////////////////////////////////////////////////
// Main

GameMode *App::GetGameMode() { return GetApp()->gameMode; }

void App::PrecacheResources()
{
	defaultBitmapFont_ = new BitmapFont();
	if ( !defaultBitmapFont_->LoadFromFile( 4, 8, "CHARSET.DTA" ) )
	{
		Error( "Failed to load default charset!\n" );
	}

	imageManager->PrecacheResources();
}

int main( int argc, char **argv )
{
	// Stop buffering stdout!
	setvbuf( stdout, nullptr, _IONBF, 0 );

	appInstance = new App( argc, argv );

	appInstance->PrecacheResources();

	appInstance->InitializeDisplay();
	appInstance->InitializeEvents();
	appInstance->InitializeGame();

	while ( appInstance->IsRunning() )
	{
		appInstance->Loop();
	}

	appInstance->Shutdown();

	return 0;
}
