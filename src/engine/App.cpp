// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "LoaderPkg.h"
#include "GameMode.h"
#include "Input/InputManager.h"
#include "Renderer/BitmapFont.h"

#include "game/DSGameMode.h"
#include "engine/Editors/SpriteSheetEditor.h"

ct::SpriteManager *ct::spriteManager = nullptr;
ct::input::InputManager *ct::input::inputManager = nullptr;

static double oldTime;

////////////////////////////////
// App Class

static ct::App *appInstance;
ct::App *ct::GetApp()
{
	return appInstance;
}

extern ALLEGRO_FILE_INTERFACE g_fsIOInterface;

///////////////////////////////////////////////////////////////////
// Memory Management

// Override C++ new/delete operators, so we can track memory usage
void *operator new( size_t size ) { return PlMAllocA( size ); }
void *operator new[]( size_t size ) { return PlMAllocA( size ); }
void operator delete( void *p ) noexcept { PlFree( p ); }
void operator delete[]( void *p ) noexcept { PlFree( p ); }
// And below is a wrapper for Allegro, so we can do the same there
static void *AlMAlloc( size_t n, int, const char *, const char * ) { return PlMAllocA( n ); }
static void AlFree( void *p, int, const char *, const char * ) { PlFree( p ); }
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

ct::App::App( int argc, char **argv )
{
	// Initialize the platform library

	pl_memory_abort_cb = MemoryAbortCallback;

	PlInitialize( argc, argv );
	PlInitializeSubSystems( PL_SUBSYSTEM_GRAPHICS | PL_SUBSYSTEM_IO );

	PlGetApplicationDataDirectory( VC_TITLE, appDataPath, sizeof( appDataPath ) );
	PlCreateDirectory( appDataPath );

	std::string logDir = std::string( appDataPath ) + "/" + VC_LOG;
	PlSetupLogOutput( logDir.c_str() );

	VC_LOG_ERR = PlAddLogLevel( "error", { 255, 0, 0 }, true );
	VC_LOG_WAR = PlAddLogLevel( "warning", { 255, 255, 0 }, true );
	VC_LOG_MSG = PlAddLogLevel( "info", { 255, 255, 255 }, true );
	VC_LOG_DEB = PlAddLogLevel( "debug", { 0, 0, 255 },
#if !defined( NDEBUG )
	                            true
#else
	                            false
#endif
	);

	Print( VC_TITLE " (build " GIT_COMMIT_COUNT " [" GIT_BRANCH ":" GIT_COMMIT_HASH "], compiled " __DATE__ ")\n" );

	PlRegisterPackageLoader( "pkg", Pkg_LoadPackage );
	PlRegisterStandardImageLoaders( PL_IMAGE_FILEFORMAT_ALL );

	PlMountLocalLocation( "./" );
	PlMountLocalLocation( appDataPath );

	bool mountLocalData = PlHasCommandLineArgument( "--mount-local-data" );
	const char *localDataPath = PlGetCommandLineArgumentValue( "--local-data-path" );

	if ( !PlLocalFileExists( "./data0.pkg" ) )
	{
		// Executable is probably under a runtime directory
		PlMountLocalLocation( "../../" );
		if ( localDataPath == nullptr )
			localDataPath = "../../data/";
	}

	if ( mountLocalData )
	{
		if ( localDataPath == nullptr )
			localDataPath = "./data/";

		PlMountLocalLocation( localDataPath );
	}

	for ( unsigned int i = 0; i < 100; ++i )
	{
		char packageName[ 64 ];
		snprintf( packageName, sizeof( packageName ), "data%d.pkg", i );
		if ( PlMountLocation( packageName ) == nullptr )
			break;
	}

	// And now initialize Allegro

	uint32_t version = al_get_allegro_version();
	uint32_t major = version >> 24;
	uint32_t minor = ( version >> 16 ) & 255;
	Print( "Initializing Allegro %d.%d\n", major, minor );
	if ( !al_init() )
		Error( "Failed to initialize Allegro library!\n" );

	static ALLEGRO_MEMORY_INTERFACE memoryInterface = {
			AlMAlloc,
			AlFree,
			AlReAlloc,
			AlCAlloc };
	al_set_memory_interface( &memoryInterface );

	if ( !al_install_mouse() )
		Error( "Failed to install mouse through Allegro!\n" );
	if ( !al_install_audio() )
		Error( "Failed to install audio through Allegro!\n" );
	if ( !al_init_acodec_addon() )
		Error( "Failed to install audio codecs through Allegro!\n" );

	al_init_native_dialog_addon();

	al_set_new_file_interface( &g_fsIOInterface );

	al_reserve_samples( 512 );

	// Doing this to ensure that rand is truly random
	// otherwise, for example, clouds will always spawn
	// in the same places every time.
	srand( ( unsigned ) time( nullptr ) );

	input::inputManager = new input::InputManager();
	spriteManager = new SpriteManager( argc, argv );

	PlRegisterConsoleVariable( "showfps", "Display the current average FPS.", "0", PL_VAR_BOOL, &debugFPS_, nullptr, false );
	PlRegisterConsoleVariable( "showprof", "Display profiler results.", "0", PL_VAR_BOOL, &debugProfiler_, nullptr, false );

	PlRegisterConsoleCommand( "newgame", "Start a new world/game.", 1,
	                          []( unsigned int argc, char **argv )
	                          {
								  IGameMode *gameMode = GetApp()->GetGameMode();
								  if ( gameMode == nullptr )
								  {
									  Warning( "No game mode is currently set!\n" );
									  return;
								  }

								  gameMode->NewGame( argv[ 1 ] );
							  } );
}

ct::App::~App() = default;

[[noreturn]] void ct::App::Loop()
{
	while ( true )
	{
		Tick();
		Draw();

		// Now clear all the performance timers
		performanceTimers.clear();
	}
}

ALLEGRO_SAMPLE *ct::App::CacheSample( const char *path )
{
	auto i = samples.find( path );
	if ( i != samples.end() )
		return i->second;

	Print( "Caching sample, \"%s\"\n", path );

	ALLEGRO_SAMPLE *sample = al_load_sample( path );
	if ( sample == nullptr )
		Error( "Failed to load sample, \"%s\"!\n", path );

	samples.emplace( path, sample );

	return sample;
}

void ct::App::ShowMessageBox( const char *title, const char *message, bool error )
{
	al_show_native_message_box(
			nullptr,
			VC_TITLE,
			title,
			message,
			nullptr,
			error ? ALLEGRO_MESSAGEBOX_ERROR : ALLEGRO_MESSAGEBOX_WARN );
}

void ct::App::Shutdown()
{
	Lisp::Uninit();

	delete gameMode_;
	delete spriteManager;
	delete input::inputManager;

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

void ct::App::InitializeDisplay()
{
	Print( "Initializing display...\n" );

#ifdef ENABLE_SCALING
	windowWidth = DISPLAY_WIDTH * 2;
	windowHeight = DISPLAY_HEIGHT * 2;
#else
	windowWidth = DISPLAY_WIDTH;
	windowHeight = DISPLAY_HEIGHT;
#endif

#ifdef ENABLE_SCALING
	int displayFlags = ALLEGRO_OPENGL_3_0;
	if ( !PlHasCommandLineArgument( "--window" ) )
		displayFlags |= ALLEGRO_FULLSCREEN_WINDOW;

	al_set_new_display_flags( displayFlags );
#else
	al_set_new_display_flags( ALLEGRO_OPENGL_3_0 );
#endif

	alDisplay = al_create_display( windowWidth, windowHeight );
	if ( alDisplay == nullptr )
		Error( "Failed to initialize display!\n" );

	// Get the actual width and height
	windowWidth = al_get_display_width( alDisplay );
	windowHeight = al_get_display_height( alDisplay );

	al_set_window_title( alDisplay, WINDOW_TITLE );

	// Check to see how much we need to scale the buffer.
	al_set_new_bitmap_flags( ALLEGRO_MEMORY_BITMAP );
	al_set_new_bitmap_format( ALLEGRO_PIXEL_FORMAT_RGB_888 );
	screenBitmap_ = al_create_bitmap( DISPLAY_WIDTH, DISPLAY_HEIGHT );
	if ( screenBitmap_ == nullptr )
		Error( "Failed to create screen buffer: %u\n", al_get_errno() );

	int sx = windowWidth / DISPLAY_WIDTH;
	int sy = windowHeight / DISPLAY_HEIGHT;
	int scale = std::min( sx, sy );

	scaleW = DISPLAY_WIDTH * scale;
	scaleH = DISPLAY_HEIGHT * scale;
	scaleX = ( windowWidth - scaleW ) / 2;
	scaleY = ( windowHeight - scaleH ) / 2;

	al_inhibit_screensaver( true );

	al_set_clipping_rectangle( 0, 0, windowWidth, windowHeight );

	redraw = true;
}

void ct::App::Draw()
{
	if ( !redraw )
		return;

	START_MEASURE();

	double newTime = PlGetCurrentSeconds();
	static unsigned int ci = 0;
	fps_[ ci++ ] = 1.0 / ( newTime - oldTime );
	if ( ci >= MAX_FPS_READINGS ) ci = 0;
	oldTime = newTime;

	// Setup the target buffer and then clear it to red
	al_set_target_bitmap( screenBitmap_ );

	// Now draw everything we want
	region_ = al_lock_bitmap( screenBitmap_, al_get_bitmap_format( screenBitmap_ ), ALLEGRO_LOCK_READWRITE );

	render::SetScissor( 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );
	render::ClearDisplay();

	if ( gameMode_ != nullptr )
		gameMode_->Draw();

	END_MEASURE();

	// Draw our debug data
	{
		char buf[ 256 ];
		int x = 8, y = 8;

		if ( debugFPS_ )
		{
			unsigned int fps = GetAverageFPS();

			snprintf( buf, sizeof( buf ), "FPS = %u\n", fps );
			hei::Colour colour;
			if ( fps <= 30 )
				colour = hei::Colour( 255, 0, 0 );
			else if ( fps <= 45 )
				colour = hei::Colour( 255, 255, 0 );
			else
				colour = hei::Colour( 0, 255, 0 );

			defaultBitmapFont_->DrawString( &x, &y, buf, colour );
		}

		if ( debugProfiler_ )
		{
			for ( auto const &i : performanceTimers )
			{
				snprintf( buf, sizeof( buf ), "%s: %f\n", i.first.c_str(), i.second.GetTimeTaken() );
				defaultBitmapFont_->DrawString( &x, &y, buf, hei::Colour( 255, 128, 50 ) );
			}
		}
	}

	console_.Draw();

	// And finally, handle the scaling
	al_unlock_bitmap( screenBitmap_ );
	al_set_target_backbuffer( alDisplay );
#ifdef ENABLE_SCALING
	al_draw_scaled_bitmap(
			screenBitmap_,
			0, 0,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			scaleX, scaleY,
			scaleW, scaleH,
			0 );
#else
	al_draw_bitmap( screenBitmap_, 0, 0, 0 );
#endif

	al_flip_display();

	redraw = false;
}

// Events

void ct::App::InitializeEvents()
{
	Print( "Initialize events...\n" );

	if ( ( alTimer = al_create_timer( 1.0 / MAX_FPS ) ) == nullptr )
		Error( "Failed to initialize timer!\n" );
	if ( ( alEventQueue = al_create_event_queue() ) == nullptr )
		Error( "Failed to initialize events!\n" );

	al_install_mouse();
	al_install_keyboard();

	debugMouse_ = PlHasCommandLineArgument( "--debug-mouse" );

	GrabCursor();

	al_register_event_source( alEventQueue, al_get_display_event_source( alDisplay ) );
	al_register_event_source( alEventQueue, al_get_timer_event_source( alTimer ) );
	al_register_event_source( alEventQueue, al_get_keyboard_event_source() );
	al_register_event_source( alEventQueue, al_get_mouse_event_source() );

	al_start_timer( alTimer );
}

void ct::App::InitializeGame()
{
	Lisp::Init();

	gameMode_ = new DSGameMode();
	gameMode_->RegisterActions();
}

void ct::App::Tick()
{
	START_MEASURE();

	ALLEGRO_EVENT event{};
	al_wait_for_event( alEventQueue, &event );

	//al_get_mouse_state( &mouseState );
	//al_get_keyboard_state( &keyboardState );

	switch ( event.type )
	{
		default: break;

		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
			al_hide_mouse_cursor( alDisplay );
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
		{
			numTicks++;

			if ( gameMode_ != nullptr )
				gameMode_->Tick();

			input::inputManager->EndFrame();
			redraw = true;
			break;
		}

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			Shutdown();
			break;

		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
		{

			break;
		}

		case ALLEGRO_EVENT_MOUSE_AXES:
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			if ( console_.IsOpen() && ( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) )
				break;

			input::InputMouseButton button = input::MOUSE_BUTTON_LEFT;
			if ( event.mouse.button & 1 )
				button = input::MOUSE_BUTTON_LEFT;
			else if ( event.mouse.button & 2 )
				button = input::MOUSE_BUTTON_RIGHT;
			else if ( event.mouse.button & 3 )
				button = input::MOUSE_BUTTON_MIDDLE;

			int aX = event.mouse.x * DISPLAY_WIDTH / windowWidth;
			int aY = event.mouse.y * DISPLAY_HEIGHT / windowHeight;
			input::inputManager->HandleMouseEvent( aX, aY, event.mouse.z, button, ( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) );
			break;
		}

		case ALLEGRO_EVENT_KEY_CHAR:
		{
			if ( !console_.IsOpen() )
				break;

			console_.PushCharacter( event.keyboard.unichar );
			break;
		}

		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_KEY_UP:
		{
			if ( console_.IsOpen() )
			{
				if ( event.type != ALLEGRO_EVENT_KEY_DOWN )
					break;

				if ( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
					console_.Close();
				else if ( event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_DOWN )
					console_.ScrollHistory( event.keyboard.keycode == ALLEGRO_KEY_UP );

				break;
			}

			if ( event.type == ALLEGRO_EVENT_KEY_DOWN && ( event.keyboard.keycode == ALLEGRO_KEY_TILDE ||
			                                               event.keyboard.keycode == ALLEGRO_KEY_ENTER ) )
			{
				console_.Open();
				break;
			}

			input::inputManager->HandleKeyboardEvent( event.keyboard.keycode, ( event.type == ALLEGRO_EVENT_KEY_UP ) );
			break;
		}
	}

	if ( !al_is_event_queue_empty( alEventQueue ) )
		redraw = false;

	END_MEASURE();
}

void ct::App::GrabCursor( bool status )
{
	// If the debug mouse mode is set, we don't grab the cursor!
	if ( debugMouse_ )
		return;

	if ( status )
	{
		al_grab_mouse( alDisplay );
		al_hide_mouse_cursor( alDisplay );
		return;
	}

	al_ungrab_mouse();
	al_show_mouse_cursor( alDisplay );
}

//////////////////////////////////////////////////////
// PROFILING

void ct::App::StartPerformanceTimer( const char *identifier )
{
	auto i = performanceTimers.find( identifier );
	if ( i == performanceTimers.end() )
	{
		performanceTimers.insert( std::pair< std::string, Timer >( identifier, Timer() ) );
		return;
	}

	i->second.Start();
}

void ct::App::EndPerformanceTimer( const char *identifier )
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

ct::IGameMode *ct::App::GetGameMode() { return GetApp()->gameMode_; }
void ct::App::SetGameMode( ct::IGameMode *gameMode )
{
	delete gameMode_;
	gameMode_ = gameMode;
}

void ct::App::Precache()
{
	defaultBitmapFont_ = new BitmapFont();
	if ( !defaultBitmapFont_->LoadFromImage( 7, 7, 0, "fonts/bitmaps/mbf/mbf_small_00.png" ) )
		Error( "Failed to load default charset!\n" );

	spriteManager->Precache();
	gameMode_->Precache();
}

int main( int argc, char **argv )
{
	// Stop buffering stdout!
	setvbuf( stdout, nullptr, _IONBF, 0 );

	appInstance = new ct::App( argc, argv );

	PlRegisterConsoleCommand( "quit", "Quit the game.", 0, []( unsigned int, char ** )
	                          { ct::GetApp()->Shutdown(); } );
	PlRegisterConsoleCommand( "editss", "Sprite sheet editor.", 0, []( unsigned int, char ** )
	                          { ct::GetApp()->SetGameMode( new ct::SpriteSheetEditor() ); } );

	appInstance->InitializeDisplay();
	appInstance->InitializeEvents();
	appInstance->InitializeGame();

	appInstance->Precache();

	oldTime = PlGetCurrentSeconds();
	appInstance->Loop();
}
