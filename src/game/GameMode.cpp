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
#include "Terrain.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Random.h"
#include "Serializer.h"
#include "BitmapFont.h"
#include "Background.h"

#include "Entities/BaseCharacter.h"

vc::GameMode::GameMode()
{
	// Cache all the data we're going to use...
#if 0
	vc::GetApp()->CacheSample( "sounds/00.wav" );
	vc::GetApp()->CacheSample( "sounds/01.wav" );
	vc::GetApp()->CacheSample( "sounds/02.wav" );
	vc::GetApp()->CacheSample( "sounds/03.wav" );
	vc::GetApp()->CacheSample( "sounds/04.wav" );
	vc::GetApp()->CacheSample( "sounds/05.wav" );
	vc::GetApp()->CacheSample( "sounds/06.wav" );

	terrainSheet = new SpriteSheet( "sheets/terrain.sdf", vc::GetApp()->CacheImage( "sheets/terrain.png" ) );
#endif

	SetupUserInterface();

	terrainManager_ = new Terrain();
	entityManager_  = new EntityManager();

	NewGame( "default.save" );
}

vc::GameMode::~GameMode()
{
	delete entityManager_;
	delete terrainManager_;
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

#define MINIMAP_WIDTH  128
#define MINIMAP_HEIGHT 128
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

	numSeconds += 10;

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

	if ( playerCamera.velocity != 0.f )
	{
		playerCamera.velocity -= ( playerCamera.velocity / CAMERA_FRICTION );
	}

#if !defined( GAME_TYPE_SFC )
	terrainManager_->Tick();
#endif
	entityManager_->Tick();

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

#if defined( GAME_TYPE_SFC )
	backgroundManager_->Draw( playerCamera );
#else
	terrainManager_->Draw( playerCamera );
#endif
	entityManager_->Draw( playerCamera );

	al_use_transform( &oldTransform );

	// UI always comes last
	if ( baseGuiPanel_ != nullptr )
	{
		baseGuiPanel_->Draw();
	}

	{
		char buf[ 256 ];
		snprintf( buf, sizeof( buf ), "D:%lu\n%02lu:%02lu:%02lu\n",
		          GetTotalDays(),
		          GetCurrentHour(), GetCurrentMinute(), GetCurrentSecond() );

		BitmapFont *font = GetApp()->GetDefaultFont();

		int x = 10, y = ( DISPLAY_HEIGHT - font->GetCharacterHeight() ) - 10;
		font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 255 ), true );
	}

	END_MEASURE();
}

void vc::GameMode::NewGame( const char *path )
{
#if !defined( GAME_TYPE_SFC )
	Print( "Generating terrain...\n" );
	terrainManager_->Generate();

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
			if ( !terrainManager_->IsWater( x, y ) )
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
		Entity *hub = entityManager_->CreateEntity( "StoreHouse" );
		hub->origin = hei::Vector2( x, y );

#define TERRITORY_BOUNDS 256

		unsigned int numAbodes = random::GenerateRandomInteger( 4, 16 );
		for ( unsigned int j = 0; j < numAbodes; ++j )
		{
			x = random::GenerateRandomInteger( territory.origin.x - TERRITORY_BOUNDS, territory.origin.x + TERRITORY_BOUNDS );
			y = random::GenerateRandomInteger( territory.origin.y - TERRITORY_BOUNDS, territory.origin.y + TERRITORY_BOUNDS );
			if ( terrainManager_->IsWater( x, y ) )
			{
				continue;
			}

			// Assign some citizens to the abode.
			int numCitizens = random::GenerateRandomInteger( 0, 4 );
			for ( unsigned int k = 0; k < numCitizens; ++k )
			{
				BaseCharacter *citizen = dynamic_cast< BaseCharacter * >( entityManager_->CreateEntity( "BaseCharacter" ) );
				if ( citizen == nullptr )
				{
					continue;
				}

				citizen->origin = hei::Vector2( x, y );
				citizen->Spawn();
			}
		}
	}
#endif


	playerCamera.position.x = 450;
	playerCamera.position.y = Background::CENTER_Y - ( DISPLAY_HEIGHT / 2 );

	// Then automatically save it
	SaveGame( path );
}

void vc::GameMode::SaveGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::WRITE );
	terrainManager_->Serialize( &serializer );
	entityManager_->SerializeEntities( &serializer );

	// Write camera data
	serializer.WriteCoordinate( playerCamera.position );
	serializer.WriteFloat( playerCamera.zoom );
	serializer.WriteFloat( playerCamera.angle );
	serializer.WriteInteger( static_cast< int >( playerCamera.movementMode ) );
}

void vc::GameMode::RestoreGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::READ );
	terrainManager_->Deserialize( &serializer );

#if 0
	entityManager_->DestroyEntities();
	entityManager_->DeserializeEntities( &serializer );
#endif

	// Now restore the camera data
	playerCamera.position     = serializer.ReadCoordinate();
	playerCamera.zoom         = serializer.ReadFloat();
	playerCamera.angle        = serializer.ReadFloat();
	playerCamera.movementMode = static_cast< Camera::MoveMode >( serializer.ReadInteger() );
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

	// And now we can do whatever

#if !defined( GAME_TYPE_SFC )
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
#endif

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
vc::Terrain       *vc::GameMode::GetTerrainManager() { return App::GetGameMode()->terrainManager_; }
vc::Background    *vc::GameMode::GetBackgroundManager() { return App::GetGameMode()->backgroundManager_; }

////////////////////////////////
// Territory

vc::GameMode::Territory::Territory( const hei::Vector2 &origin )
{
	snprintf( name, sizeof( name ), "T%d", rand() % 100 );
}

void vc::GameMode::Territory::DrawBorder()
{
}
