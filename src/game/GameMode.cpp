// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GameMode.h"
#include "Terrain.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"
#include "BitmapFont.h"
#include "Background.h"

#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "GUI/GUIStyleSheet.h"

#include "Entities/BaseCharacter.h"

vc::GameMode::GameMode()
{
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

	uiDefaultStyleSheet = new GUIStyleSheet();
	if ( !uiDefaultStyleSheet->LoadFile( "gui/skins/default.txt" ) )
	{
		Error( "Failed to load default style sheet!\n" );
	}

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

	// Create the UI cursor
	new GUICursor( baseGuiPanel_ );

	uiPieMenu = new GUIPieMenu( baseGuiPanel_ );
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

	if ( world_ != nullptr )
	{
		world_->Tick();
		entityManager_->Tick();
	}

	END_MEASURE();
}

void vc::GameMode::Draw()
{
	START_MEASURE();

	//	backgroundManager_->Draw( playerCamera );
	//	entityManager_->Draw( playerCamera );

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

	if ( world_ != nullptr )
	{
		char buf[ 256 ];
		snprintf( buf, sizeof( buf ), "DAY %lu\nH%02u:M%02u:S%02u\n",
		          world_->GetTotalDays(),
		          world_->GetCurrentHour(),
		          world_->GetCurrentMinute(),
		          world_->GetCurrentSecond() );

		int x = 10, y = ( DISPLAY_HEIGHT - font->GetCharacterHeight() ) - 20;
		font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 255 ), true );
	}

	vc::spriteManager->DrawSprite( "sprites/ui/icon_talk.png", SpriteManager::SPRITE_GROUP_GUI, 256, 256 );

	END_MEASURE();
}

void vc::GameMode::NewGame( const char *path )
{
#if 0
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
			x = random::GenerateRandomInteger( 0, Terrain::PIXEL_WIDTH );
			y = random::GenerateRandomInteger( 0, Terrain::PIXEL_HEIGHT );
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

#	define TERRITORY_BOUNDS 256

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

	playerCamera.position.x = 450;
	playerCamera.position.y = Background::CENTER_Y - ( DISPLAY_HEIGHT / 2 );

	entityManager_->CreateEntity( "BoidManager" );

	// Then automatically save it
	SaveGame( path );
#endif
}

void vc::GameMode::SaveGame( const char *path )
{
	Serializer serializer( path, Serializer::Mode::WRITE );

	//terrainManager_->Serialize( &serializer );
	entityManager_->SerializeEntities( &serializer );

	// World state
	world_->Serialize( &serializer );

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

	char tmp[ 128 ];
	serializer.ReadString( tmp, sizeof( tmp ) );
	world_ = new World( tmp );
	world_->Deserialize( &serializer );

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
vc::Terrain       *vc::GameMode::GetTerrainManager() { return App::GetGameMode()->terrainManager_; }
vc::Background    *vc::GameMode::GetBackgroundManager() { return App::GetGameMode()->backgroundManager_; }

#if 0
void vc::GameMode::DrawRoomsDebug( const vc::Camera &camera )
{
	if ( !enableRoomDraw_ )
	{
		return;
	}

	BitmapFont *font = GetApp()->GetDefaultFont();

	for ( auto room : rooms_ )
	{
		DrawFilledRectangle( room.x - ( int ) camera.position.x,
		                     room.y - ( int ) camera.position.y,
		                     room.w - room.x,
		                     room.h - room.y,
		                     hei::Colour( 0, 0, 128, 100 ) );

		int tx = room.x - ( int ) camera.position.x;
		int ty = room.y - ( int ) camera.position.y;
		for ( unsigned int i = 0; i < 16; ++i )
		{
			al_put_pixel( tx, ty + i, al_map_rgb( 0, 0, 255 ) );
			al_put_pixel( tx + i, ty, al_map_rgb( 0, 0, 255 ) );
		}

		// w/h are explicit
		tx = room.w - ( int ) camera.position.x;
		ty = room.h - ( int ) camera.position.y;
		for ( unsigned int i = 0; i < 16; ++i )
		{
			al_put_pixel( tx, ty - i, al_map_rgb( 0, 0, 255 ) );
			al_put_pixel( tx - i, ty, al_map_rgb( 0, 0, 255 ) );
		}

		tx = ( ( room.w - room.x ) / 2 ) + room.x - ( int ) camera.position.x;
		ty = ( ( room.h - room.y ) / 2 ) + room.y - ( int ) camera.position.y;

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
#endif
