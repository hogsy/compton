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

#include "Compton.h"
#include "GameMode.h"
#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"
#include "BitmapFont.h"
#include "Background.h"
#include "render.h"

vc::GameMode::GameMode()
{
	SetupUserInterface();

	entityManager_ = new EntityManager();

	NewGame( "default.save" );
}

vc::GameMode::~GameMode()
{
	delete entityManager_;
	delete backgroundManager_;
	delete baseGuiPanel_;
}

void vc::GameMode::SetupUserInterface()
{
	// Now create the base GUI panels

	baseGuiPanel_ = new GUIPanel( nullptr, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );

#if !defined( GAME_TYPE_SFC )
	uiDefaultStyleSheet = new GUIStyleSheet( "sheets/interface.sdf", vc::GetApp()->CacheImage( "sheets/interface.png" ) );
	baseGuiPanel_->SetStyleSheet( uiDefaultStyleSheet );

	new GUIButton( baseGuiPanel_, "Hello World", 2, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 34, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 66, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", 2, 34, 96, 32 );

	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 2, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 34, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 66, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 34, 32, 32 );

#	define MINIMAP_WIDTH  128
#	define MINIMAP_HEIGHT 128
	GUIPanel *minimapPanel = new GUIPanel(
	        baseGuiPanel_,
	        DISPLAY_WIDTH - MINIMAP_WIDTH - 2,
	        DISPLAY_HEIGHT - MINIMAP_HEIGHT - 2,
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

void vc::GameMode::Tick()
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

	// Restrict the camera to the world bounds
	if ( playerCamera.position.x + DISPLAY_WIDTH < 0.0f )
	{
		playerCamera.position.x = Background::PIXEL_WIDTH;
	}
	else if ( playerCamera.position.x > Background::PIXEL_WIDTH )
	{
		playerCamera.position.x = -DISPLAY_WIDTH;
	}

	if ( playerCamera.position.y < 0.0f )
	{
		playerCamera.position.y = 0.0f;
	}
	else if ( playerCamera.position.y + DISPLAY_HEIGHT > Background::PIXEL_HEIGHT )
	{
		playerCamera.position.y = Background::PIXEL_HEIGHT - DISPLAY_HEIGHT;
	}

	if ( playerCamera.velocity.x != 0 || playerCamera.velocity.y != 0 )
	{
		playerCamera.velocity -= ( playerCamera.velocity / CAMERA_FRICTION );
	}

	numSeconds += 15;

	entityManager_->Tick();

	END_MEASURE();
}

void vc::GameMode::Draw()
{
	START_MEASURE();

	backgroundManager_->Draw( playerCamera );
	entityManager_->Draw( playerCamera );

	DrawRoomsDebug( playerCamera );

	// UI always comes last
	if ( baseGuiPanel_ != nullptr )
	{
		baseGuiPanel_->Draw();
	}

	BitmapFont *font = GetApp()->GetDefaultFont();
	if ( enableHelpPrompt_ )
	{
		int x = 10;
		int y = DISPLAY_HEIGHT / 2;
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

		int x = 10, y = ( DISPLAY_HEIGHT - font->GetCharacterHeight() ) - 20;
		font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 255 ), true );
	}

	END_MEASURE();
}

void vc::GameMode::NewGame( const char *path )
{
	LoadRooms();

	playerCamera.position.x = 450;
	playerCamera.position.y = Background::CENTER_Y - ( DISPLAY_HEIGHT / 2 );

	entityManager_->CreateEntity( "BoidManager" );

	// Then automatically save it
	SaveGame( path );
}

void vc::GameMode::SaveGame( const char *path )
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

void vc::GameMode::RestoreGame( const char *path )
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

hei::Vector2 vc::GameMode::MousePosToWorld( int x, int y ) const
{
	return hei::Vector2( ( playerCamera.position.x - DISPLAY_WIDTH / 2 ) + x, ( playerCamera.position.y - DISPLAY_HEIGHT / 2 ) + y );
}

void vc::GameMode::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( baseGuiPanel_ != nullptr && baseGuiPanel_->HandleMouseEvent( x, y, wheel, button, buttonUp ) )
	{
		return;
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
}

void vc::GameMode::HandleKeyboardEvent( int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( baseGuiPanel_ != nullptr && baseGuiPanel_->HandleKeyboardEvent( button, buttonUp ) )
	{
		return;
	}

	if ( buttonUp )
	{
		return;
	}

	switch ( button )
	{
		default:
			break;

		case ALLEGRO_KEY_R:
			enableRoomDraw_ = !enableRoomDraw_;
			break;
		case ALLEGRO_KEY_H:
			enableHelpPrompt_ = !enableHelpPrompt_;
			break;
		case ALLEGRO_KEY_Q:
			GetApp()->Shutdown();
			break;

		case ALLEGRO_KEY_F5:
		{
			// Quick save check
			SaveGame( "quick.save" );
			break;
		}
		case ALLEGRO_KEY_F6:
		{
			// Quick load check
			RestoreGame( "quick.save" );
			break;
		}

		case ALLEGRO_KEY_ESCAPE:
		case ALLEGRO_KEY_PAUSE:
		{
			// TODO: pause/unpause sounds
			if ( gameState == GameState::PAUSED )
			{
				gameState = GameState::ACTIVE;
				break;
			}

			gameState = GameState::PAUSED;
			break;
		}
	}
}

vc::PlayerManager *vc::GameMode::GetPlayerManager() { return App::GetGameMode()->playerManager; }
vc::EntityManager *vc::GameMode::GetEntityManager() { return App::GetGameMode()->entityManager_; }
vc::Background    *vc::GameMode::GetBackgroundManager() { return App::GetGameMode()->backgroundManager_; }

void vc::GameMode::LoadRooms()
{
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

void vc::GameMode::DrawRoomsDebug( const vc::Camera &camera )
{
	if ( !enableRoomDraw_ )
	{
		return;
	}

	BitmapFont *font = GetApp()->GetDefaultFont();

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
const vc::GameMode::Room *vc::GameMode::GetRoom( int x, int y ) const
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

const vc::GameMode::Room *vc::GameMode::GetRoomByType( uint16_t type, unsigned int startIndex ) const
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

const vc::GameMode::Room *vc::GameMode::GetRoomByIndex( unsigned int index ) const
{
	if ( index >= rooms_.size() )
	{
		return nullptr;
	}

	return &rooms_[ index ];
}

////////////////////////////////
// Territory

vc::GameMode::Territory::Territory( const hei::Vector2 &origin )
{
	snprintf( name, sizeof( name ), "T%d", rand() % 100 );
}

void vc::GameMode::Territory::DrawBorder()
{
}
