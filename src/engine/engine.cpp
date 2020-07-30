/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"

#include "ImageBitmap.h"
#include "LoaderPkg.h"
#include "GameMode.h"
#include "EntityManager.h"
#include "agent.h"

#include "vm/vm.h"

// Draw Routines

void DrawBitmap( ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h ) {
	if ( bitmap == nullptr ) {
		return;
	}

	float bW = al_get_bitmap_width( bitmap );
	float bH = al_get_bitmap_height( bitmap );

	al_draw_scaled_bitmap( bitmap, 0, 0, bW, bH, x, y, w, h, 0 );
}

void DrawString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message ) {
	if ( font == nullptr ) {
		return;
	}

	al_draw_text( font, colour, x, y, ALLEGRO_ALIGN_LEFT, message );
}

void DrawShadowString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message ) {
	if ( font == nullptr ) {
		return;
	}

	al_draw_text( font, al_map_rgb( 0, 0, 0 ), x + 1, y + 1, ALLEGRO_ALIGN_LEFT, message );
	al_draw_text( font, colour, x, y, ALLEGRO_ALIGN_LEFT, message );
}

void DrawCenteredString( const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message ) {
	if ( font == nullptr ) {
		return;
	}
	al_draw_text( font, colour, x, y, ALLEGRO_ALIGN_CENTER, message );
}

void DrawFilledRectangle( PLVector2 position, float w, float h, ALLEGRO_COLOR colour ) {
	al_draw_filled_rectangle( position.x, position.y, w, h, colour );
}

void DrawVerticalGradientRectangle( float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom ) {
	ALLEGRO_VERTEX v[ 4 ];
	memset( v, 0, sizeof( ALLEGRO_VERTEX ) * 4 );
	v[ 0 ].x = x;
	v[ 0 ].y = y;
	v[ 0 ].color = top;
	v[ 1 ].x = x + w;
	v[ 1 ].y = y;
	v[ 1 ].color = top;
	v[ 2 ].x = x;
	v[ 2 ].y = y + h;
	v[ 2 ].color = bottom;
	v[ 3 ].x = x + w;
	v[ 3 ].y = y + h;
	v[ 3 ].color = bottom;

	al_draw_prim( v, nullptr, nullptr, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP );
}

////////////////////////////////
// App Class

static vc::App *appInstance;
vc::App *vc::GetApp() {
	return appInstance;
}

#define VC_LOG "debug"
#define VC_TITLE "SimGame"
#define VC_VERSION "Pre-Alpha v0.1.0"

vc::App::App( int argc, char **argv ) {
	// Initialize the platform library

	plInitialize( argc, argv );

	plGetApplicationDataDirectory( VC_TITLE, appDataPath, sizeof( appDataPath ) );
	plCreateDirectory( appDataPath );

	std::string logDir = std::string( appDataPath ) + "/" + VC_LOG;
	plSetupLogOutput( logDir.c_str() );

	plSetupLogLevel( VC_LOG_ERR, "error", { 255, 0, 0 }, true );
	plSetupLogLevel( VC_LOG_WAR, "warning", { 255, 255, 0 }, true );
	plSetupLogLevel( VC_LOG_MSG, "info", { 255, 255, 255 }, true );
	plSetupLogLevel( VC_LOG_DEB, "debug", { 0, 0, 255 }, true );

	plRegisterPackageLoader( "pkg", Pkg_LoadPackage );

	plMountLocalLocation( appDataPath );
	plMountLocalLocation( "data/" );

	for ( unsigned int i = 0; i < 100; ++i ) {
		char packageName[ 64 ];
		snprintf( packageName, sizeof( packageName ), "data%d.pkg", i );
		if ( plMountLocation( packageName ) == nullptr ) {
			break;
		}
	}

	Print( "SimGame " VC_VERSION " (" __DATE__ ")\n" );

	// And now initialize Allegro

	uint32_t version = al_get_allegro_version();
	uint32_t major = version >> 24;
	uint32_t minor = ( version >> 16 ) & 255;
	Print( "Initializing Allegro %d.%d\n", major, minor );
	if ( !al_init() ) {
		Error( "Failed to initialize Allegro library!\n" );
	}

	if ( !al_install_mouse() ) {
		Error( "Failed to install mouse through Allegro!\n" );
	}

	if ( !al_install_audio() ) {
		Error( "Failed to install audio through Allegro!\n" );
	}

	if ( !al_init_acodec_addon() ) {
		Error( "Failed to install audio codecs through Allegro!\n" );
	}

	al_init_native_dialog_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	al_register_bitmap_loader( ".gfx", ImageBitmap_LoadPacked );
	al_register_bitmap_loader( ".png", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".bmp", ImageBitmap_LoadGeneric );
	al_register_bitmap_loader( ".tga", ImageBitmap_LoadGeneric );

	al_reserve_samples( 512 );

	ALLEGRO_PATH *path = al_get_standard_path( ALLEGRO_RESOURCES_PATH );
	al_change_directory( al_path_cstr( path, '/' ) );
	al_destroy_path( path );

	// And now, finally, finish setting up the engine

	VM_Initialize();

	// Doing this to ensure that rand is truly random
	// otherwise, for example, clouds will always spawn
	// in the same places every time.
	srand( ( unsigned ) time( nullptr ) );

	running = true;
}

vc::App::~App() {
	Shutdown();
}

bool vc::App::IsRunning() {
	// We're always running!
	return true;
}

void vc::App::Loop() {
	Tick();
	Draw();
}

ALLEGRO_FONT *vc::App::CacheFont( const char *path, unsigned int size ) {
	std::string fullName = std::string( path ) + ":" + std::to_string( size );
	auto i = fonts.find( fullName );
	if ( i != fonts.end() ) {
		return i->second;
	}

	Print( "Caching font, \"%s\" size %d\n", fullName.c_str(), size );

	ALLEGRO_FONT *font = al_load_ttf_font( path, size, 0 );
	if ( font == nullptr ) {
		Error( "Failed to load font, \"%s\"!\n", path );
	}

	fonts.emplace( fullName, font );

	return font;
}

ALLEGRO_SAMPLE *vc::App::CacheSample( const char *path ) {
	auto i = samples.find( path );
	if ( i != samples.end() ) {
		return i->second;
	}

	Print( "Caching sample, \"%s\"\n", path );

	ALLEGRO_SAMPLE *sample = al_load_sample( path );
	if ( sample == nullptr ) {
		Error( "Failed to load sample, \"%s\"!\n", path );
	}

	samples.emplace( path, sample );

	return sample;
}

ALLEGRO_BITMAP *vc::App::CacheImage( const char *path ) {
	auto i = bitmaps.find( path );
	if ( i != bitmaps.end() ) {
		return i->second;
	}

	Print( "Caching image, \"%s\"\n", path );

	ALLEGRO_BITMAP *bitmap = al_load_bitmap( path );
	if ( bitmap == nullptr ) {
		Error( "Failed to load bitmap, \"%s\"!\n", path );
	}

	bitmaps.emplace( path, bitmap );

	return bitmap;
}

void vc::App::ShowMessageBox( const char *title, const char *message, bool error ) {
	al_show_native_message_box(
			nullptr,
			VC_TITLE,
			title,
			message,
			nullptr,
			error ? ALLEGRO_MESSAGEBOX_ERROR : ALLEGRO_MESSAGEBOX_WARN );
}

void vc::App::Shutdown() {
	delete gameMode;

	if ( alDisplay != nullptr ) {
		al_destroy_display( alDisplay );
		alDisplay = nullptr;
	}

	if ( alEventQueue != nullptr ) {
		al_destroy_event_queue( alEventQueue );
		alEventQueue = nullptr;
	}

	if ( alTimer != nullptr ) {
		al_destroy_timer( alTimer );
		alTimer = nullptr;
	}
}

// Display

void vc::App::InitializeDisplay() {
	Print( "Initializing display...\n" );

	windowWidth = WINDOW_WIDTH;
	windowHeight = WINDOW_HEIGHT;

	al_set_new_display_flags( ALLEGRO_WINDOWED | ALLEGRO_OPENGL );
	alDisplay = al_create_display( windowWidth, windowHeight );
	if ( alDisplay == nullptr ) {
		Error( "Failed to initialize display!\n" );
	}

	al_set_window_title( alDisplay, WINDOW_TITLE );

	// Check to see how much we need to scale the buffer.
	int flags = al_get_new_bitmap_flags();
	//al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);
	buffer = al_create_bitmap( DISPLAY_WIDTH, DISPLAY_HEIGHT );
	al_set_new_bitmap_flags( flags );
	int sx = windowWidth / DISPLAY_WIDTH;
	int sy = windowHeight / DISPLAY_HEIGHT;
	int scale = std::min( sx, sy );

	scaleW = DISPLAY_WIDTH * scale;
	scaleH = DISPLAY_HEIGHT * scale;
	scaleX = ( windowWidth - scaleW ) / 2;
	scaleY = ( windowHeight - scaleH ) / 2;

	al_inhibit_screensaver( true );

	al_set_clipping_rectangle( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

	redraw = true;
}

void vc::App::Draw() {
	if ( !redraw ) {
		return;
	}

	// Setup the target buffer and then clear it to red
	al_set_target_bitmap( buffer );
	al_clear_to_color( al_map_rgb( 128, 0, 0 ) );

	// Now draw everything we want

	AgentFactory::Get()->Draw();

	// And finally, handle the scaling
	al_set_target_backbuffer( alDisplay );
	al_draw_scaled_bitmap(
			buffer,
			0, 0,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
#if 0
			engine_vars.scalex, engine_vars.scaley,
            engine_vars.scalew, engine_vars.scaleh,
#else
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
#endif
			0 );

	al_flip_display();

	redraw = false;
}

// Events

void vc::App::InitializeEvents() {
	Print( "Initialize Events\n" );

	if ( ( alTimer = al_create_timer( 1.0 / 60 ) ) == nullptr ) {
		Error( "Failed to initialize timer!\n" );
	}

	if ( ( alEventQueue = al_create_event_queue() ) == nullptr ) {
		Error( "Failed to initialize events!\n" );
	}

	al_install_mouse();
	al_install_keyboard();

#if 0// enable once we're happy with everything else.
	al_hide_mouse_cursor(engine_vars.display);
#endif

	al_register_event_source( alEventQueue, al_get_display_event_source( alDisplay ) );
	al_register_event_source( alEventQueue, al_get_timer_event_source( alTimer ) );
	al_register_event_source( alEventQueue, al_get_keyboard_event_source() );

	al_start_timer( alTimer );
}

void vc::App::InitializeGame() {
	entityManager = new EntityManager();
	gameMode = new GameMode();
}

void vc::App::Tick() {
	ALLEGRO_EVENT event{};
	al_wait_for_event( alEventQueue, &event );

	al_get_mouse_state( &mouseState );
	al_get_keyboard_state( &keyboardState );

	mouseStatus[ MOUSE_BUTTON_LEFT ] = ( mouseState.buttons & 1 ) != 0;
	mouseStatus[ MOUSE_BUTTON_RIGHT ] = ( mouseState.buttons & 2 ) != 0;
	mouseStatus[ MOUSE_BUTTON_MIDDLE ] = ( mouseState.buttons & 3 ) != 0;

	switch ( event.type ) {
		default:break;

		case ALLEGRO_EVENT_TIMER:
			numTicks++;
			gameMode->Tick();
			redraw = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			running = false;
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			gameMode->HandleMouseEvent( event.mouse.x, event.mouse.y, event.mouse.button, false );
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			gameMode->HandleMouseEvent( event.mouse.x, event.mouse.y, event.mouse.button, true );
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			keyStatus[ event.keyboard.keycode ] = true;
			gameMode->HandleKeyboardEvent( event.keyboard.keycode, false );
			break;
		case ALLEGRO_EVENT_KEY_UP:
			keyStatus[ event.keyboard.keycode ] = false;
			gameMode->HandleKeyboardEvent( event.keyboard.keycode, true );
			break;
	}

	if ( !al_is_event_queue_empty( alEventQueue ) ) {
		redraw = false;
	}
}

////////////////////////////////
// Main

int main( int argc, char **argv ) {
	// Stop buffering stdout!
	setvbuf( stdout, nullptr, _IONBF, 0 );

	appInstance = new vc::App( argc, argv );

	appInstance->InitializeDisplay();
	appInstance->InitializeEvents();
	appInstance->InitializeGame();

	while ( appInstance->IsRunning() ) {
		appInstance->Loop();
	}

	appInstance->Shutdown();

	return 0;
}