// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "LoaderPkg.h"
#include "GameMode.h"
#include "EntityManager.h"
#include "BitmapFont.h"

vc::SpriteManager *vc::spriteManager = nullptr;

static double oldTime;

// Draw Routines

void DrawPixel( int x, int y, const hei::Colour &colour )
{
	if ( x < 0 || x > DISPLAY_WIDTH )
	{
		return;
	}

	if ( y < 0 || y > DISPLAY_HEIGHT )
	{
		return;
	}

	if ( colour.a != 255 )
	{
		al_put_blended_pixel( x, y, al_map_rgba( colour.r, colour.g, colour.b, colour.a ) );
	}
	else
	{
		al_put_pixel( x, y, al_map_rgb( colour.r, colour.g, colour.b ) );
	}
}

void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest )
{
	if ( ( x + w < 0 || x >= DISPLAY_WIDTH ) || ( y + h < 0 || y >= DISPLAY_HEIGHT ) )
	{
		return;
	}

	struct RGBA8
	{
		uint8_t r, g, b, a;
	};

	int dw = w;
	if ( x + dw > DISPLAY_WIDTH )
	{
		dw = DISPLAY_WIDTH - x;
	}
	int dh = h;
	if ( y + dh > DISPLAY_HEIGHT )
	{
		dh = DISPLAY_HEIGHT - y;
	}

	bool hasAlpha = ( pixelSize == 4 );
	ALLEGRO_LOCKED_REGION *region = vc::GetApp()->region_;
	uint8_t *dst = ( uint8_t * ) region->data + x * region->pixel_size + region->pitch * y;
	for ( unsigned int c = 0; c < dh; ++c )
	{
		for ( unsigned int r = 0; r < dw; ++r )
		{
			RGBA8 pixel = ( const struct RGBA8 & ) pixels[ ( r + c * w ) * pixelSize ];
			if ( alphaTest && hasAlpha && ( pixel.a != 255 ) )
			{
				if ( pixel.a == 0 )
				{
					continue;
				}

				al_put_blended_pixel( x, y, al_map_rgba( pixel.r, pixel.g, pixel.b, pixel.a ) );
				continue;
			}

			dst[ r * 3 + 0 ] = pixel.b;
			dst[ r * 3 + 1 ] = pixel.g;
			dst[ r * 3 + 2 ] = pixel.r;
		}

		dst += DISPLAY_WIDTH * region->pixel_size;
	}
}

void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour )
{
	if ( colour.a == 0 )
	{
		return;
	}

	for ( int row = 0; row < w; ++row )
	{
		for ( int column = 0; column < h; ++column )
		{
			DrawPixel( x + row, y + column, colour );
		}
	}
}

////////////////////////////////
// App Class

static vc::App *appInstance;
vc::App *vc::GetApp()
{
	return appInstance;
}

extern ALLEGRO_FILE_INTERFACE g_fsIOInterface;

///////////////////////////////////////////////////////////////////
// Memory Management

// Override C++ new/delete operators, so we can track memory usage
void *operator new( size_t size ) { return PlMAllocA( size ); }
void *operator new[]( size_t size ) { return PlMAllocA( size ); }
void operator delete( void *p ) throw() { PlFree( p ); }
void operator delete[]( void *p ) throw() { PlFree( p ); }
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

vc::App::App( int argc, char **argv )
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
		{
			localDataPath = "../../data/";
		}
	}

	if ( mountLocalData )
	{
		if ( localDataPath == nullptr )
		{
			localDataPath = "./data/";
		}

		PlMountLocalLocation( localDataPath );
	}

	for ( unsigned int i = 0; i < 100; ++i )
	{
		char packageName[ 64 ];
		snprintf( packageName, sizeof( packageName ), "data%d.pkg", i );
		if ( PlMountLocation( packageName ) == nullptr )
		{
			break;
		}
	}

	// And now initialize Allegro

	uint32_t version = al_get_allegro_version();
	uint32_t major = version >> 24;
	uint32_t minor = ( version >> 16 ) & 255;
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

	if ( !al_install_audio() )
	{
		Error( "Failed to install audio through Allegro!\n" );
	}

	if ( !al_init_acodec_addon() )
	{
		Error( "Failed to install audio codecs through Allegro!\n" );
	}

	al_init_native_dialog_addon();

	al_set_new_file_interface( &g_fsIOInterface );
	al_register_bitmap_loader( ".png", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".bmp", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".tga", ImageBitmap_LoadGeneric );

	al_reserve_samples( 512 );

	// Doing this to ensure that rand is truly random
	// otherwise, for example, clouds will always spawn
	// in the same places every time.
	srand( ( unsigned ) time( nullptr ) );

	spriteManager = new SpriteManager( argc, argv );

	running = true;
}

vc::App::~App() = default;

bool vc::App::IsRunning()
{
	// We're always running!
	return true;
}

void vc::App::Loop()
{
	Tick();
	Draw();

	// Now clear all the performance timers
	performanceTimers.clear();
}

ALLEGRO_SAMPLE *vc::App::CacheSample( const char *path )
{
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
}

void vc::App::ShowMessageBox( const char *title, const char *message, bool error )
{
	al_show_native_message_box(
			nullptr,
			VC_TITLE,
			title,
			message,
			nullptr,
			error ? ALLEGRO_MESSAGEBOX_ERROR : ALLEGRO_MESSAGEBOX_WARN );
}

void vc::App::Shutdown()
{
	delete gameMode;
	delete spriteManager;

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

	exit( 0 );
}

// Display

void vc::App::InitializeDisplay()
{
	Print( "Initializing display...\n" );

	windowWidth = DISPLAY_WIDTH;
	windowHeight = DISPLAY_HEIGHT;

	//al_set_new_display_flags( ALLEGRO_FULLSCREEN_WINDOW );
	alDisplay = al_create_display( windowWidth, windowHeight );
	if ( alDisplay == nullptr )
	{
		Error( "Failed to initialize display!\n" );
	}

	// Get the actual width and height
	windowWidth = al_get_display_width( alDisplay );
	windowHeight = al_get_display_height( alDisplay );

	al_set_window_title( alDisplay, WINDOW_TITLE );

	// Check to see how much we need to scale the buffer.
	al_set_new_bitmap_flags( ALLEGRO_MEMORY_BITMAP );
	al_set_new_bitmap_format( ALLEGRO_PIXEL_FORMAT_RGB_888 );
	screenBitmap_ = al_create_bitmap( DISPLAY_WIDTH, DISPLAY_HEIGHT );
	if ( screenBitmap_ == nullptr )
	{
		Error( "Failed to create screen buffer: %u\n", al_get_errno() );
	}

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

void vc::App::Draw()
{
	if ( !redraw )
	{
		return;
	}

	double newTime = Timer::GetCurrentTime();
	fps_ = 1.0 / ( newTime - oldTime );
	oldTime = newTime;

	// Setup the target buffer and then clear it to red
	al_set_target_bitmap( screenBitmap_ );
	al_clear_to_color( al_map_rgb( 0, 0, 128 ) );

	// Now draw everything we want

	region_ = al_lock_bitmap( screenBitmap_, al_get_bitmap_format( screenBitmap_ ), ALLEGRO_LOCK_READWRITE );

	gameMode->Draw();

	// Draw our debug data
	{
		char buf[ 256 ];
		int x = 8, y = 8;

		snprintf( buf, sizeof( buf ), "FPS = %u\n", ( unsigned int ) fps_ );
		hei::Colour colour;
		if ( fps_ <= 30 )
		{
			colour = hei::Colour( 255, 0, 0 );
		}
		else if ( fps_ <= 45 )
		{
			colour = hei::Colour( 255, 255, 0 );
		}
		else
		{
			colour = hei::Colour( 0, 255, 0 );
		}
		defaultBitmapFont_->DrawString( &x, &y, buf, colour );

		for ( auto const &i : performanceTimers )
		{

			snprintf( buf, sizeof( buf ), "%s: %f\n", i.first.c_str(), i.second.GetTimeTaken() );
			defaultBitmapFont_->DrawString( &x, &y, buf, hei::Colour( 255, 128, 50 ) );
		}
	}

	al_unlock_bitmap( screenBitmap_ );

	// And finally, handle the scaling
	al_set_target_backbuffer( alDisplay );
	al_draw_scaled_bitmap(
			screenBitmap_,
			0, 0,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			scaleX, scaleY,
			scaleW, scaleH,
			0 );

	al_flip_display();

	redraw = false;
}

// Events

void vc::App::InitializeEvents()
{
	Print( "Initialize events...\n" );

	if ( ( alTimer = al_create_timer( 1.0 / MAX_FPS ) ) == nullptr )
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

void vc::App::InitializeGame()
{
	gameMode = new GameMode();
}

void vc::App::Tick()
{
	START_MEASURE();

	ALLEGRO_EVENT event{};
	al_wait_for_event( alEventQueue, &event );

	al_get_mouse_state( &mouseState );
	al_get_keyboard_state( &keyboardState );

	mouseStatus[ MOUSE_BUTTON_LEFT ] = ( mouseState.buttons & 1 ) != 0;
	mouseStatus[ MOUSE_BUTTON_RIGHT ] = ( mouseState.buttons & 2 ) != 0;
	mouseStatus[ MOUSE_BUTTON_MIDDLE ] = ( mouseState.buttons & 3 ) != 0;

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

		case ALLEGRO_EVENT_MOUSE_AXES:
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			if ( gameMode == nullptr )
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

			int aX = event.mouse.x * DISPLAY_WIDTH / windowWidth;
			int aY = event.mouse.y * DISPLAY_HEIGHT / windowHeight;
			gameMode->HandleMouseEvent( aX, aY, event.mouse.dz, button, ( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) );
			break;
		}

		case ALLEGRO_EVENT_KEY_DOWN:
			if ( gameMode == nullptr )
			{
				break;
			}
			keyStatus[ event.keyboard.keycode ] = true;
			gameMode->HandleKeyboardEvent( event.keyboard.keycode, false );
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if ( gameMode == nullptr )
			{
				break;
			}
			keyStatus[ event.keyboard.keycode ] = false;
			gameMode->HandleKeyboardEvent( event.keyboard.keycode, true );
			break;
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
void vc::App::GetCursorPosition( int *dX, int *dY ) const
{
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state( &state );

	*dX = state.x * DISPLAY_WIDTH / windowWidth;
	*dY = state.y * DISPLAY_HEIGHT / windowHeight;
}

bool vc::App::GetKeyState( int key ) const
{
	assert( key > 0 && key < ALLEGRO_KEY_MAX );
	return keyStatus[ key ];
}

bool vc::App::GetMouseState( int *dX, int *dY, InputMouseButton button )
{
	if ( dX != nullptr && dY != nullptr )
	{
		GetCursorPosition( dX, dY );
	}

	return mouseStatus[ button ];
}

void vc::App::GrabCursor( bool status )
{
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

void vc::App::StartPerformanceTimer( const char *identifier )
{
	performanceTimers.insert( std::pair< std::string, Timer >( identifier, Timer() ) );
}

void vc::App::EndPerformanceTimer( const char *identifier )
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

vc::GameMode *vc::App::GetGameMode() { return GetApp()->gameMode; }

void vc::App::PrecacheResources()
{
	defaultBitmapFont_ = new BitmapFont();
	if ( !defaultBitmapFont_->LoadFromImage( 7, 7, 0, "fonts/bitmaps/mbf/mbf_small_00.png" ) )
	{
		Error( "Failed to load default charset!\n" );
	}

	spriteManager->PrecacheResources();
}

int main( int argc, char **argv )
{
	// Stop buffering stdout!
	setvbuf( stdout, nullptr, _IONBF, 0 );

	appInstance = new vc::App( argc, argv );

	appInstance->PrecacheResources();

	appInstance->InitializeDisplay();
	appInstance->InitializeEvents();
	appInstance->InitializeGame();

	oldTime = Timer::GetCurrentTime();
	while ( appInstance->IsRunning() )
	{
		appInstance->Loop();
	}

	appInstance->Shutdown();

	return 0;
}
