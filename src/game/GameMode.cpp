/*
SimGame Engine
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

#include "SimGame.h"
#include "GameMode.h"
#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "Terrain.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Random.h"
#include "Serializer.h"

#include "Entities/BaseCharacter.h"

vc::GameMode::GameMode()
{
	// Cache all the data we're going to use...
	vc::GetApp()->CacheFont( "fonts/ps2p/PressStart2P.ttf", FONT_SIZE_LARGE );
	vc::GetApp()->CacheFont( "fonts/ps2p/PressStart2P.ttf", FONT_SIZE_SMALL );
	vc::GetApp()->CacheFont( "fonts/ps2p/PressStart2P.ttf", 32 );
	vc::GetApp()->CacheFont( "fonts/ps2p/PressStart2P.ttf", FONT_SIZE_MEDIUM );

	vc::GetApp()->CacheSample( "sounds/00.wav" );
	vc::GetApp()->CacheSample( "sounds/01.wav" );
	vc::GetApp()->CacheSample( "sounds/02.wav" );
	vc::GetApp()->CacheSample( "sounds/03.wav" );
	vc::GetApp()->CacheSample( "sounds/04.wav" );
	vc::GetApp()->CacheSample( "sounds/05.wav" );
	vc::GetApp()->CacheSample( "sounds/06.wav" );

	SetupUserInterface();

	terrainSheet = new SpriteSheet( "sheets/terrain.sdf", vc::GetApp()->CacheImage( "sheets/terrain.png" ) );

	terrainManager = new Terrain();
	entityManager  = new EntityManager();

	NewGame( "default.save" );
}

vc::GameMode::~GameMode()
{
	delete entityManager;
	delete terrainManager;
	delete uiBasePanelPtr;
}

void vc::GameMode::SetupUserInterface()
{
	uiDefaultStyleSheet = new GUIStyleSheet( "sheets/interface.sdf", vc::GetApp()->CacheImage( "sheets/interface.png" ) );

	// Now create the base GUI panels

	uiBasePanelPtr = new GUIPanel( nullptr, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );
	uiBasePanelPtr->SetStyleSheet( uiDefaultStyleSheet );

	new GUIButton( uiBasePanelPtr, "Hello World", 2, 2, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", 34, 2, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", 66, 2, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", 2, 34, 96, 32 );

	new GUIButton( uiBasePanelPtr, "Hello World", DISPLAY_WIDTH - 34, 2, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", DISPLAY_WIDTH - 34, 34, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", DISPLAY_WIDTH - 34, 66, 32, 32 );
	new GUIButton( uiBasePanelPtr, "Hello World", DISPLAY_WIDTH - 34, 34, 32, 32 );

#define MINIMAP_WIDTH  128
#define MINIMAP_HEIGHT 128
	GUIPanel *minimapPanel = new GUIPanel(
			uiBasePanelPtr,
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

	// Create the UI cursor
	new GUICursor( uiBasePanelPtr );

	uiPieMenu = new GUIPieMenu( uiBasePanelPtr );
}

void vc::GameMode::Tick()
{
	START_MEASURE();

	// Always tick UI, because we still want to access it even if paused
	uiBasePanelPtr->Tick();

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
	if ( playerCamera.velocity != 0.f )
	{
		playerCamera.velocity -= ( playerCamera.velocity / CAMERA_FRICTION );
	}

#if 0
	// Restrict the camera to the world bounds
	if ( playerCamera.position.x < 0.0f ) {
		playerCamera.position.x = 0.0f;
		playerCamera.velocity.x = 0.0f;
	} else if ( playerCamera.position.x + DISPLAY_WIDTH > TERRAIN_PIXEL_WIDTH ) {
		playerCamera.position.x = playerCamera.position.x + DISPLAY_WIDTH;
		playerCamera.velocity.x = 0.0f;
	}
	if ( playerCamera.position.y < 0.0f ) {
		playerCamera.position.y = 0.0f;
		playerCamera.velocity.y = 0.0f;
	} else if ( playerCamera.position.y + DISPLAY_HEIGHT > TERRAIN_PIXEL_HEIGHT ) {
		playerCamera.position.y = playerCamera.position.y + DISPLAY_HEIGHT;
		playerCamera.velocity.y = 0.0f;
	}
#endif

	terrainManager->Tick();
	entityManager->Tick();

	END_MEASURE();
}

void vc::GameMode::Draw()
{
	START_MEASURE();

	ALLEGRO_TRANSFORM transform, oldTransform;
	oldTransform = *al_get_current_transform();

	al_identity_transform( &transform );

	al_rotate_transform( &transform, playerCamera.angle );
	al_scale_transform( &transform, playerCamera.zoom, playerCamera.zoom );
	al_translate_transform( &transform, -playerCamera.position.x, -playerCamera.position.y );
	al_translate_transform( &transform, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 );

	al_use_transform( &transform );

	terrainManager->Draw( playerCamera );
	entityManager->Draw( playerCamera );

	al_use_transform( &oldTransform );

	// UI always comes last
	uiBasePanelPtr->Draw();

	al_draw_textf( GetApp()->GetDefaultFont(), al_map_rgb( 255, 0, 0 ), 0.0f, 128.0f, 0, "camera: %f %f %f", playerCamera.position.x, playerCamera.position.y, playerCamera.zoom );

	unsigned int xr = PlRoundUp( playerCamera.position.x * TERRAIN_TILE_WIDTH / TERRAIN_PIXEL_WIDTH, 1 );
	unsigned int yr = PlRoundUp( playerCamera.position.y * TERRAIN_TILE_HEIGHT / TERRAIN_PIXEL_HEIGHT, 1 );
	al_draw_textf( GetApp()->GetDefaultFont(), al_map_rgb( 255, 255, 255 ), 0.0f, 256.0f, 0, "camera_tile: %d %d", xr, yr );

	END_MEASURE();
}

void vc::GameMode::NewGame( const char *path )
{
	Print( "Generating terrain...\n" );
	terrainManager->Generate();

	Print( "Generating territories...\n" );
	unsigned int n = random::GenerateRandomInteger( 4, 8 );
	for ( unsigned int i = 0; i < n; ++i )
	{
		// Pick a random point in the world
		float x, y;
		for ( unsigned int j = 0; j < 16; ++j )
		{
			// Try at least 16 times before we give up
			x = random::GenerateRandomInteger( 0, TERRAIN_PIXEL_WIDTH );
			y = random::GenerateRandomInteger( 0, TERRAIN_PIXEL_HEIGHT );
			if ( !terrainManager->IsWater( x, y ) )
			{
				break;
			}

			x = y = -1.0f;
		}

		// Never found a point without water, give up...
		if ( x == -1.0f && y == -1.0f )
		{
			continue;
		}

		// Now attempt to spawn in the territory
		Territory territory( hei::Vector2( x, y ) );

		// Spawn one Storehouse at the center
		Entity *hub = entityManager->CreateEntity( "StoreHouse" );
		hub->origin = hei::Vector2( x, y );

#define TERRITORY_BOUNDS 256

		unsigned int numAbodes = random::GenerateRandomInteger( 4, 16 );
		for ( unsigned int j = 0; j < numAbodes; ++j )
		{
			x = random::GenerateRandomInteger( territory.origin.x - TERRITORY_BOUNDS, territory.origin.x + TERRITORY_BOUNDS );
			y = random::GenerateRandomInteger( territory.origin.y - TERRITORY_BOUNDS, territory.origin.y + TERRITORY_BOUNDS );
			if ( terrainManager->IsWater( x, y ) )
			{
				continue;
			}

			// Assign some citizens to the abode.
			int numCitizens = random::GenerateRandomInteger( 0, 4 );
			for ( unsigned int k = 0; k < numCitizens; ++k )
			{
				BaseCharacter *citizen = dynamic_cast< BaseCharacter * >( entityManager->CreateEntity( "BaseCharacter" ) );
				if ( citizen == nullptr )
				{
					continue;
				}

				citizen->origin = hei::Vector2( x, y );
				citizen->Spawn();
			}
		}
	}

	// Set the camera to the middle of the world
	playerCamera.position.x = TERRAIN_PIXEL_WIDTH / 2;
	playerCamera.position.y = TERRAIN_PIXEL_HEIGHT / 2;

	// Then automatically save it
	SaveGame( path );
}

void vc::GameMode::SaveGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::WRITE );
	terrainManager->Serialize( &serializer );
	entityManager->SerializeEntities( &serializer );

	// Write camera data
	serializer.WriteCoordinate( playerCamera.position );
	serializer.WriteFloat( playerCamera.zoom );
	serializer.WriteFloat( playerCamera.angle );
	serializer.WriteInteger( static_cast< int >( playerCamera.movementMode ) );
}

void vc::GameMode::RestoreGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::READ );
	terrainManager->Deserialize( &serializer );

#if 0
	entityManager->DestroyEntities();
	entityManager->DeserializeEntities( &serializer );
#endif

	// Now restore the camera data
	playerCamera.position	  = serializer.ReadCoordinate();
	playerCamera.zoom		  = serializer.ReadFloat();
	playerCamera.angle		  = serializer.ReadFloat();
	playerCamera.movementMode = static_cast< Camera::MoveMode >( serializer.ReadInteger() );
}

hei::Vector2 vc::GameMode::MousePosToWorld( int x, int y ) const
{
	return hei::Vector2( ( playerCamera.position.x - DISPLAY_WIDTH / 2 ) + x, ( playerCamera.position.y - DISPLAY_HEIGHT / 2 ) + y );
}

void vc::GameMode::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( uiBasePanelPtr != nullptr && uiBasePanelPtr->HandleMouseEvent( x, y, wheel, button, buttonUp ) )
	{
		return;
	}

	// And now we can do whatever

	if ( wheel > 0 )
	{
		playerCamera.zoom += 0.015f;
		if ( playerCamera.zoom > 1.0f )
		{
			playerCamera.zoom = 1.0f;
		}
		return;
	}
	else if ( wheel < 0 )
	{
		playerCamera.zoom -= 0.015f;
		if ( playerCamera.zoom < 0.2f )
		{
			playerCamera.zoom = 0.2f;
		}
		return;
	}

	static Entity *waypoint = nullptr;
	if ( GetApp()->GetMouseState( &x, &y, MOUSE_BUTTON_LEFT ) && !buttonUp )
	{
		if ( waypoint == nullptr )
		{
			waypoint = entityManager->CreateEntity( "DebugWaypoint" );
		}

		waypoint->origin = MousePosToWorld( x, y );
	}
	else if ( GetApp()->GetMouseState( &x, &y, MOUSE_BUTTON_RIGHT ) && !buttonUp && ( waypoint != nullptr ) )
	{
		entityManager->DestroyEntity( waypoint );
		waypoint = nullptr;
	}
}

void vc::GameMode::HandleKeyboardEvent( int button, bool buttonUp )
{
	// Push input through to GUI first, so that can do whatever it needs to
	if ( uiBasePanelPtr != nullptr && uiBasePanelPtr->HandleKeyboardEvent( button, buttonUp ) )
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
vc::EntityManager *vc::GameMode::GetEntityManager() { return App::GetGameMode()->entityManager; }
vc::Terrain		*vc::GameMode::GetTerrainManager() { return App::GetGameMode()->terrainManager; }

////////////////////////////////
// Territory

vc::GameMode::Territory::Territory( const hei::Vector2 &origin )
{
	snprintf( name, sizeof( name ), "T%d", rand() % 100 );
}

void vc::GameMode::Territory::DrawBorder()
{
}
