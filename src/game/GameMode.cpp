// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GameMode.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"
#include "BitmapFont.h"
#include "Background.h"

#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "GUI/GUIStyleSheet.h"

#include "Input/InputManager.h"
#include "LispInterface.h"

ct::GameMode::GameMode()
{
	SetupUserInterface();

	entityManager_ = new EntityManager();
}

ct::GameMode::~GameMode()
{
	delete entityManager_;
	delete backgroundManager_;
	delete baseGuiPanel_;
}

void ct::GameMode::SetupUserInterface()
{
	// Now create the base GUI panels

	baseGuiPanel_ = new GUIPanel( nullptr, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );

	uiDefaultStyleSheet = new GUIStyleSheet();
	if ( !uiDefaultStyleSheet->LoadFile( "gui/skins/default.txt" ) )
	{
		Error( "Failed to load default style sheet!\n" );
	}

	baseGuiPanel_->SetStyleSheet( uiDefaultStyleSheet );

	//for ( unsigned int i = 0; i < 256; ++i )
	//{
	//	GUIPanel *statusBar_ = new GUIPanel( baseGuiPanel_, 0, i * 16, DISPLAY_WIDTH, 16, GUIPanel::Background::DEFAULT, GUIPanel::Border::OUTSET );
	//}

	GUIPanel *statusBar_ = new GUIPanel( baseGuiPanel_, 0, 0, DISPLAY_WIDTH, 16, GUIPanel::Background::DEFAULT, GUIPanel::Border::OUTSET );
	statusBar_->SetBackgroundColour( hei::Colour( 0, 0, 0, 255 ) );

	//new GUIButton( baseGuiPanel_, "Hello World", 2, 2, 32, 32 );
	//new GUIButton( baseGuiPanel_, "Hello World", 34, 2, 32, 32 );
	//new GUIButton( baseGuiPanel_, "Hello World", 66, 2, 32, 32 );
	//new GUIButton( baseGuiPanel_, "Hello World", 2, 34, 96, 32 );

	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 22, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 44, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 76, 32, 32 );
	new GUIButton( baseGuiPanel_, "Hello World", DISPLAY_WIDTH - 34, 108, 32, 32 );

#if 1
#	define MINIMAP_WIDTH  128
#	define MINIMAP_HEIGHT 128
	GUIPanel *minimapPanel = new GUIPanel(
			baseGuiPanel_,
			DISPLAY_WIDTH - MINIMAP_WIDTH - 2,
			DISPLAY_HEIGHT - MINIMAP_HEIGHT - 2,
			MINIMAP_WIDTH, MINIMAP_HEIGHT,
			GUIPanel::Background::DEFAULT,
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

	uiPieMenu = new GUIPieMenu( baseGuiPanel_ );
}

void ct::GameMode::PrecacheResources()
{
	entityManager_->PrecacheEntities();
}

void ct::GameMode::Tick()
{
	START_MEASURE();

	// Always tick UI, because we still want to access it even if paused
	if ( baseGuiPanel_ != nullptr )
	{
		baseGuiPanel_->Tick();
	}

	if ( gameState_ == GameState::PAUSED )
	{
		END_MEASURE();
		return;
	}

	if ( camera.movementMode == Camera::MoveMode::FREE )
	{
		if ( input::inputManager->IsKeyDown( ALLEGRO_KEY_UP ) )
		{
			camera.velocity.y -= CAMERA_ACCELERATION;
			if ( camera.velocity.y > CAMERA_MAXSPEED )
			{
				camera.velocity.y = CAMERA_MAXSPEED;
			}
		}
		else if ( input::inputManager->IsKeyDown( ALLEGRO_KEY_DOWN ) )
		{
			camera.velocity.y += CAMERA_ACCELERATION;
			if ( camera.velocity.y < -CAMERA_MAXSPEED )
			{
				camera.velocity.y = -CAMERA_MAXSPEED;
			}
		}

		if ( input::inputManager->IsKeyDown( ALLEGRO_KEY_LEFT ) )
		{
			camera.velocity.x -= CAMERA_ACCELERATION;
			if ( camera.velocity.x > CAMERA_MAXSPEED )
			{
				camera.velocity.x = CAMERA_MAXSPEED;
			}
		}
		else if ( input::inputManager->IsKeyDown( ALLEGRO_KEY_RIGHT ) )
		{
			camera.velocity.x += CAMERA_ACCELERATION;
			if ( camera.velocity.x < -CAMERA_MAXSPEED )
			{
				camera.velocity.x = -CAMERA_MAXSPEED;
			}
		}

		camera.oldPosition = camera.position;
		camera.position += camera.velocity;

		// Restrict the camera to the world bounds
		if ( camera.position.x + DISPLAY_WIDTH < 0.0f )
		{
			camera.position.x = Background::PIXEL_WIDTH;
		}
		else if ( camera.position.x > Terrain::PIXEL_WIDTH )
		{
			camera.position.x = -DISPLAY_WIDTH;
		}
		if ( camera.position.y < 0.0f )
		{
			camera.position.y = 0.0f;
		}
		else if ( camera.position.y + DISPLAY_HEIGHT > Terrain::PIXEL_HEIGHT )
		{
			camera.position.y = Terrain::PIXEL_HEIGHT - DISPLAY_HEIGHT;
		}

		if ( camera.velocity.x != 0 || camera.velocity.y != 0 )
		{
			camera.velocity -= ( camera.velocity / CAMERA_FRICTION );
		}
	}

	if ( world_ != nullptr )
		world_->Tick();

	GetEntityManager()->Tick();

	END_MEASURE();
}

void ct::GameMode::Draw()
{
	START_MEASURE();

	if ( world_ != nullptr )
	{
		world_->Draw( camera );
	}

	entityManager_->Draw( camera );

	// UI always comes last
	if ( baseGuiPanel_ != nullptr )
	{
		ct::GetApp()->StartPerformanceTimer( "GUI Draw" );
		baseGuiPanel_->Draw();
		ct::GetApp()->EndPerformanceTimer( "GUI Draw" );
	}

	BitmapFont *font = GetApp()->GetDefaultFont();

	if ( world_ != nullptr )
	{
		char buf[ 256 ];
		snprintf( buf, sizeof( buf ), "DAY %u\nH%02u:M%02u:S%02u\n",
		          world_->GetTotalDays(),
		          world_->GetCurrentHour(),
		          world_->GetCurrentMinute(),
		          world_->GetCurrentSecond() );

		int x = 10, y = ( DISPLAY_HEIGHT - font->GetCharacterHeight() ) - 20;
		font->DrawString( &x, &y, buf, hei::Colour( 255, 128, 255 ), true );
	}

	END_MEASURE();
}

void ct::GameMode::NewGame( const char *path )
{
	world_ = new World( "test" );
	world_->Generate( rand() % 255 );

	// Then automatically save it
	SaveGame( path );

	entityManager_->SpawnEntities();

	LI_CompileScript( "test.lsp" );
}

void ct::GameMode::SaveGame( const char *path )
{
	// No point saving if there's no world!
	if ( world_ == nullptr )
		return;

	Serializer serializer( path, Serializer::Mode::WRITE );

	//terrainManager_->Serialize( &serializer );
	entityManager_->SerializeEntities( &serializer );

	// World state
	world_->Serialize( &serializer );

	camera.Serialize( &serializer );

	Print( "Game saved to \"%s\"\n", path );
}

void ct::GameMode::RestoreGame( const char *path )
{
	delete world_;

	entityManager_->DestroyEntities();

	Serializer serializer( path, Serializer::Mode::READ );

	//terrainManager_->Deserialize( &serializer );
	entityManager_->DeserializeEntities( &serializer );

	char tmp[ 128 ];
	serializer.ReadString( tmp, sizeof( tmp ) );
	world_ = new World( tmp );
	world_->Deserialize( &serializer );

	// Now restore the camera data
	camera.position = serializer.ReadCoordinate();
	camera.movementMode = static_cast< Camera::MoveMode >( serializer.ReadI32() );

	entityManager_->SpawnEntities();

	Print( "Game restored from \"%s\"\n", path );
}

hei::Vector2 ct::GameMode::MousePosToWorld( int x, int y ) const
{
	return hei::Vector2( ( camera.position.x - DISPLAY_WIDTH / 2 ) + x, ( camera.position.y - DISPLAY_HEIGHT / 2 ) + y );
}

void ct::GameMode::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
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

void ct::GameMode::HandleKeyboardEvent( int button, bool buttonUp )
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
			if ( gameState_ == GameState::PAUSED )
			{
				gameState_ = GameState::ACTIVE;
				break;
			}

			gameState_ = GameState::PAUSED;
			break;
		}
	}
}

ct::PlayerManager *ct::GameMode::GetPlayerManager() { return App::GetGameMode()->playerManager; }
ct::EntityManager *ct::GameMode::GetEntityManager() { return App::GetGameMode()->entityManager_; }
ct::Background *ct::GameMode::GetBackgroundManager() { return App::GetGameMode()->backgroundManager_; }

////////////////////////////////////////////////
// Actions

void ct::GameMode::RegisterActions()
{
	actions_[ ACTION_MOVE_UP ] = input::inputManager->PushAction( "Move Up" );
	actions_[ ACTION_MOVE_UP ]->BindKey( ALLEGRO_KEY_UP );

	actions_[ ACTION_MOVE_DOWN ] = input::inputManager->PushAction( "Move Down" );
	actions_[ ACTION_MOVE_DOWN ]->BindKey( ALLEGRO_KEY_DOWN );

	actions_[ ACTION_MOVE_LEFT ] = input::inputManager->PushAction( "Move Left" );
	actions_[ ACTION_MOVE_LEFT ]->BindKey( ALLEGRO_KEY_LEFT );

	actions_[ ACTION_MOVE_RIGHT ] = input::inputManager->PushAction( "Move Right" );
	actions_[ ACTION_MOVE_RIGHT ]->BindKey( ALLEGRO_KEY_RIGHT );

	actions_[ ACTION_USE ] = input::inputManager->PushAction( "Use" );
	actions_[ ACTION_USE ]->BindKey( ALLEGRO_KEY_V );

	actions_[ ACTION_ATTACK ] = input::inputManager->PushAction( "Attack" );
	actions_[ ACTION_ATTACK ]->BindKey( ALLEGRO_KEY_SPACE );
}

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
