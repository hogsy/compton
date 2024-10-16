// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include <allegro5/keycodes.h>

#include "plcore/pl_filesystem.h"

#include "app.h"
#include "serializer.h"
#include "game_mode.h"
#include "background.h"

#include "entity/entity_manager.h"
#include "entity/entity.h"

#include "render/render_bitmap_font.h"
#include "render/render.h"

#include "GUI/GUIPanel.h"
#include "GUI/GUICursor.h"

GameMode *GetGameModeInterface()
{
	static GameMode *gameMode;
	if ( gameMode == nullptr )
	{
		gameMode = new GameMode();
	}

	return gameMode;
}

GameMode::GameMode()
{
	SetupUserInterface();

	entityManager_     = new EntityManager();
	backgroundManager_ = new Background();

	NewGame( "default.save" );
}

GameMode::~GameMode()
{
	delete entityManager_;
	delete backgroundManager_;
	delete baseGuiPanel_;
}

void GameMode::SetupUserInterface()
{
	// Now create the base GUI panels

	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->get_draw_height();

	baseGuiPanel_ = new GUIPanel( nullptr, 0, 0, sw, sh );
#if 0
	uiDefaultStyleSheet = new GUIStyleSheet( "sheets/interface.sdf", GetApp()->CacheImage( "sheets/interface.png" ) );
	baseGuiPanel_->SetStyleSheet( uiDefaultStyleSheet );

	new GUIButton( baseGuiPanel_, "Hello World", 2, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 34, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 66, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 2, 34, 96, 32 );

	new GUIButton( baseGuiPanel_, "Hello World", sw - 34, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", sw - 34, 34, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", sw - 34, 66, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", sw - 34, 34, 32, 32 );

#	define MINIMAP_WIDTH  128
#	define MINIMAP_HEIGHT 128
	GUIPanel *minimapPanel = new GUIPanel(
	        baseGuiPanel_,
	        sw - MINIMAP_WIDTH - 2,
	        sh - MINIMAP_HEIGHT - 2,
	        MINIMAP_WIDTH, MINIMAP_HEIGHT,
	        GUIPanel::Background::TEXTURE,
	        GUIPanel::Border::OUTSET );
	minimapPanel->SetBackground( GUIPanel::Background::NONE );
	minimapPanel->SetBorder( GUIPanel::Border::OUTSET );
	new GUIPanel(
	        minimapPanel,
	        2, 2,
	        MINIMAP_WIDTH - 4, MINIMAP_HEIGHT - 4,
	        GUIPanel::Background::SOLID,
	        GUIPanel::Border::INSET );
#endif

	// Create the UI cursor
	new GUICursor( baseGuiPanel_ );

	//uiPieMenu = new GUIPieMenu( baseGuiPanel_ );
}

void GameMode::Tick()
{
	START_MEASURE();

	// Always tick UI, because we still want to access it even if paused
	if ( baseGuiPanel_ != nullptr )
	{
		baseGuiPanel_->Tick();
	}

	if ( gameState == GameState::PAUSED )
	{
		END_MEASURE();
		return;
	}

#define CAMERA_MAXSPEED     10.0f
#define CAMERA_ACCELERATION 2.0f
#define CAMERA_FRICTION     10.0f

	if ( GetApp()->GetKeyState( ALLEGRO_KEY_UP ) )
	{
		playerCamera.velocity.y -= CAMERA_ACCELERATION;
		if ( playerCamera.velocity.y > CAMERA_MAXSPEED )
		{
			playerCamera.velocity.y = CAMERA_MAXSPEED;
		}
	}
	else if ( GetApp()->GetKeyState( ALLEGRO_KEY_DOWN ) )
	{
		playerCamera.velocity.y += CAMERA_ACCELERATION;
		if ( playerCamera.velocity.y < -CAMERA_MAXSPEED )
		{
			playerCamera.velocity.y = -CAMERA_MAXSPEED;
		}
	}

	if ( GetApp()->GetKeyState( ALLEGRO_KEY_LEFT ) )
	{
		playerCamera.velocity.x -= CAMERA_ACCELERATION;
		if ( playerCamera.velocity.x > CAMERA_MAXSPEED )
		{
			playerCamera.velocity.x = CAMERA_MAXSPEED;
		}
	}
	else if ( GetApp()->GetKeyState( ALLEGRO_KEY_RIGHT ) )
	{
		playerCamera.velocity.x += CAMERA_ACCELERATION;
		if ( playerCamera.velocity.x < -CAMERA_MAXSPEED )
		{
			playerCamera.velocity.x = -CAMERA_MAXSPEED;
		}
	}

	playerCamera.oldPosition = playerCamera.position;
	playerCamera.position += playerCamera.velocity;

	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->get_draw_height();

	// Restrict the camera to the world bounds
	playerCamera.position.x = PlClamp( 0.0f, playerCamera.position.x, backgroundManager_->get_width() - sw );
	playerCamera.position.y = PlClamp( 0.0f, playerCamera.position.y, backgroundManager_->get_height() - sh );

	if ( playerCamera.velocity.x != 0 || playerCamera.velocity.y != 0 )
	{
		playerCamera.velocity -= ( playerCamera.velocity / CAMERA_FRICTION );
	}

	numSeconds += 15;

	entityManager_->Tick();

	END_MEASURE();
}

void GameMode::Draw()
{
	START_MEASURE();

	int sh = GetApp()->get_draw_height();

	backgroundManager_->draw( playerCamera, false );
	entityManager_->draw( playerCamera );
	backgroundManager_->draw( playerCamera, true );

	DrawRoomsDebug( playerCamera );

	// UI always comes last
	if ( baseGuiPanel_ != nullptr )
	{
		baseGuiPanel_->Draw();
	}

	BitmapFont *font = GetApp()->get_default_font();
	if ( enableHelpPrompt_ )
	{
		int x = 10;
		int y = sh / 2;
		font->DrawString( &x, &y, "HELP INFO:\n"
		                          "Press R to display room volumes\n"
		                          "Press H to toggle this help\n"
		                          "Press Q to quit\n" );
	}

	{
		char buf[ 256 ];
		snprintf( buf, sizeof( buf ), "DAY %lu\nH%02u:M%02u:S%02u\n",
		          GetTotalDays(),
		          GetCurrentHour(), GetCurrentMinute(), GetCurrentSecond() );

		int x = 10, y = ( sh - font->GetCharacterHeight() ) - 20;
		font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 255 ), true );
	}

	END_MEASURE();
}

void GameMode::NewGame( const char *path )
{
	LoadRooms();

	int sh                  = GetApp()->get_draw_height();
	playerCamera.position.x = 450;
	playerCamera.position.y = ( backgroundManager_->get_height() / 2 ) - ( sh / 2 );

	entityManager_->CreateEntity( "BoidManager" );

	// Then automatically save it
	SaveGame( path );
}

void GameMode::SaveGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::WRITE );

	//terrainManager_->Serialize( &serializer );
	entityManager_->SerializeEntities( &serializer );

	// World state
	serializer.WriteInteger( numSeconds );

	// Write camera data
	serializer.WriteCoordinate( playerCamera.position );
	serializer.WriteInteger( static_cast< int >( playerCamera.movementMode ) );

	Print( "Game saved to \"%s\"\n", path );
}

void GameMode::RestoreGame( const char *path )
{
	entityManager_->DestroyEntities();

	Serializer serializer( path, Serializer::Mode::READ );

	//terrainManager_->Deserialize( &serializer );
	entityManager_->DeserializeEntities( &serializer );

	// World state
	numSeconds = serializer.ReadInteger();

	// Now restore the camera data
	playerCamera.position     = serializer.ReadCoordinate();
	playerCamera.movementMode = static_cast< Camera::MoveMode >( serializer.ReadInteger() );

	Print( "Game restored from \"%s\"\n", path );
}

hei::Vector2 GameMode::MousePosToWorld( int x, int y ) const
{
	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->get_draw_height();

	return hei::Vector2( ( playerCamera.position.x - sw / 2 ) + x, ( playerCamera.position.y - sh / 2 ) + y );
}

bool GameMode::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( baseGuiPanel_ != nullptr && baseGuiPanel_->HandleMouseEvent( x, y, wheel, button, buttonUp ) )
	{
		return false;
	}

#if 0
	static Entity *waypoint = nullptr;
	if ( GetApp()->GetMouseState( &x, &y, MOUSE_BUTTON_LEFT ) && !buttonUp )
	{
		if ( waypoint == nullptr )
		{
			waypoint = entityManager_->CreateEntity( "DebugWaypoint" );
		}

		waypoint->origin = MousePosToWorld( x, y );
	}
	else if ( GetApp()->GetMouseState( &x, &y, MOUSE_BUTTON_RIGHT ) && !buttonUp && ( waypoint != nullptr ) )
	{
		entityManager_->DestroyEntity( waypoint );
		waypoint = nullptr;
	}
#endif

	return false;
}

bool GameMode::HandleKeyboardEvent( int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( baseGuiPanel_ != nullptr && baseGuiPanel_->HandleKeyboardEvent( button, buttonUp ) )
	{
		return false;
	}

	if ( buttonUp )
	{
		return false;
	}

	switch ( button )
	{
		default:
			break;

		case ALLEGRO_KEY_R:
			enableRoomDraw_ = !enableRoomDraw_;
			return true;
		case ALLEGRO_KEY_H:
			enableHelpPrompt_ = !enableHelpPrompt_;
			return true;
		case ALLEGRO_KEY_Q:
			GetApp()->Shutdown();
			return true;

		case ALLEGRO_KEY_F5:
		{
			// Quick save check
			SaveGame( "quick.save" );
			return true;
		}
		case ALLEGRO_KEY_F6:
		{
			// Quick load check
			RestoreGame( "quick.save" );
			return true;
		}

		case ALLEGRO_KEY_ESCAPE:
		case ALLEGRO_KEY_PAUSE:
		{
			// TODO: pause/unpause sounds
			if ( gameState == GameState::PAUSED )
			{
				gameState = GameState::ACTIVE;
				return true;
			}

			gameState = GameState::PAUSED;
			return true;
		}
	}

	return false;
}

PlayerManager *GameMode::GetPlayerManager() { return ( ( GameMode * ) App::GetGameMode() )->playerManager; }
EntityManager *GameMode::GetEntityManager() { return ( ( GameMode * ) App::GetGameMode() )->entityManager_; }
Background    *GameMode::GetBackgroundManager() { return ( ( GameMode    *) App::GetGameMode() )->backgroundManager_; }

void GameMode::LoadRooms()
{
	if ( !backgroundManager_->load( "sprites/background" ) )
	{
		Error( "Failed to load collision background!\n" );
	}

	Print( "Loading rooms...\n" );

	PLFile *file = PlOpenFile( "ROOMS.DTA", false );
	if ( file == nullptr )
	{
		Warning( "Failed to load rooms: %s\n", PlGetError() );
		return;
	}

	uint16_t numRooms = ( uint16_t ) PlReadInt16( file, false, nullptr );
	for ( unsigned int i = 0; i < numRooms; ++i )
	{
		rooms_.push_back( {
		        PlReadInt16( file, false, nullptr ),             // x
		        PlReadInt16( file, false, nullptr ),             // w
		        PlReadInt16( file, false, nullptr ),             // y
		        PlReadInt16( file, false, nullptr ),             // h
		        ( uint16_t ) PlReadInt16( file, false, nullptr ),// type
		        i                                                // id
		} );
	}

	PlCloseFile( file );

	Print( "Read %lu rooms successfully!\n", numRooms );
}

void GameMode::DrawRoomsDebug( const Camera &camera )
{
	if ( !enableRoomDraw_ )
	{
		return;
	}

	BitmapFont *font = GetApp()->get_default_font();

	for ( auto room : rooms_ )
	{
		render::DrawFilledRectangle( room.x - ( int ) camera.position.x,
		                             room.y - ( int ) camera.position.y,
		                             room.w - room.x,
		                             room.h - room.y,
		                             hei::Colour( 0, 0, 128, 100 ) );

		int tx = ( ( room.w - room.x ) / 2 ) + room.x - ( int ) camera.position.x;
		int ty = ( ( room.h - room.y ) / 2 ) + room.y - ( int ) camera.position.y;

		static const char *roomType[] = {
		        "INTERIOR",
		        "EXTERIOR",
		};

		char buf[ 32 ];
		snprintf( buf, sizeof( buf ), "ROOM (%s)\n", roomType[ room.type ] );
		size_t l   = strlen( buf );
		int    scx = tx - ( ( font->GetCharacterWidth() * l ) / 2 );

		font->DrawString( &scx, &ty, buf );
	}
}

/**
 * Get room by given coords, useful for checking if an entity
 * is occupying that space.
 */
const GameMode::Room *GameMode::GetRoom( int x, int y ) const
{
	for ( auto &room : rooms_ )
	{
		if ( x >= room.x && x < room.w && y >= room.y && y < room.h )
		{
			return &room;
		}
	}

	return nullptr;
}

const GameMode::Room *GameMode::GetRoomByType( uint16_t type, unsigned int startIndex ) const
{
	if ( startIndex >= rooms_.size() )
	{
		return nullptr;
	}

	for ( unsigned int i = startIndex; i < rooms_.size(); ++i )
	{
		if ( rooms_[ i ].type != type )
		{
			continue;
		}

		return &rooms_[ i ];
	}

	return nullptr;
}

const GameMode::Room *GameMode::GetRoomByIndex( unsigned int index ) const
{
	if ( index >= rooms_.size() )
	{
		return nullptr;
	}

	return &rooms_[ index ];
}
