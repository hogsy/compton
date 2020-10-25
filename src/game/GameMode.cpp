/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "GameMode.h"
#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "Terrain.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"

vc::GameMode::GameMode() {
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
	entityManager = new EntityManager();

	NewGame( "ExampleSave.save" );
}

vc::GameMode::~GameMode() {
	delete entityManager;
	delete terrainManager;
	delete uiBasePanelPtr;
}

void vc::GameMode::SetupUserInterface() {
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

#define MINIMAP_WIDTH   128
#define MINIMAP_HEIGHT  128
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
}

void vc::GameMode::Tick() {
	START_MEASURE();

	// Always tick UI, because we still want to access it even if paused
	uiBasePanelPtr->Tick();

	if ( gameState == GameState::PAUSED ) {
		END_MEASURE();
		return;
	}

	if ( GetApp()->GetKeyState( ALLEGRO_KEY_UP ) ) {
		playerCamera.velocity.y += CAMERA_ACCELERATION;
		if ( playerCamera.velocity.y > CAMERA_MAXSPEED ) {
			playerCamera.velocity.y = CAMERA_MAXSPEED;
		}
	} else if ( GetApp()->GetKeyState( ALLEGRO_KEY_DOWN ) ) {
		playerCamera.velocity.y -= CAMERA_ACCELERATION;
		if ( playerCamera.velocity.y < -CAMERA_MAXSPEED ) {
			playerCamera.velocity.y = -CAMERA_MAXSPEED;
		}
	}

	if ( GetApp()->GetKeyState( ALLEGRO_KEY_LEFT ) ) {
		playerCamera.velocity.x += CAMERA_ACCELERATION;
		if ( playerCamera.velocity.x > CAMERA_MAXSPEED ) {
			playerCamera.velocity.x = CAMERA_MAXSPEED;
		}
	} else if ( GetApp()->GetKeyState( ALLEGRO_KEY_RIGHT ) ) {
		playerCamera.velocity.x -= CAMERA_ACCELERATION;
		if ( playerCamera.velocity.x < -CAMERA_MAXSPEED ) {
			playerCamera.velocity.x = -CAMERA_MAXSPEED;
		}
	}

	playerCamera.oldPosition = playerCamera.position;
	playerCamera.position += playerCamera.velocity;
	if ( playerCamera.velocity != 0.f ) {
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

void vc::GameMode::Draw() {
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

	END_MEASURE();
}

void vc::GameMode::NewGame( const char *path ) {
	// todo: Generate environment

	terrainManager->Generate();

	for ( unsigned int i = 0; i < 2048; ++i ) {
		Entity *entity = entityManager->CreateEntity( "BaseCharacter" );
		entity->Spawn();

		entity->origin.x = rand() % TERRAIN_PIXEL_WIDTH;
		entity->origin.y = rand() % TERRAIN_PIXEL_HEIGHT;
	}

	// Then automatically save it
	SaveGame( path );
}

void vc::GameMode::SaveGame( const char *path ) {
	Serializer serializer( path, Serializer::Mode::WRITE );
	terrainManager->Serialize( &serializer );
	entityManager->SerializeEntities( &serializer );
}

void vc::GameMode::RestoreGame( const char *path ) {
	Serializer serializer( path, Serializer::Mode::READ );
	terrainManager->Deserialize( &serializer );

#if 0
	entityManager->DestroyEntities();
	entityManager->DeserializeEntities( &serializer );
#endif
}

void vc::GameMode::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp ) {
	// Push input through to GUI first, so that can do whatever it needs to
	if ( uiBasePanelPtr->HandleMouseEvent( x, y, wheel, button, buttonUp ) ) {
		return;
	}

	// And now we can do whatever

	if ( wheel > 0 ) {
		playerCamera.zoom += 0.015f;
		if ( playerCamera.zoom > 1.0f ) {
			playerCamera.zoom = 1.0f;
		}
		return;
	} else if ( wheel < 0 ) {
		playerCamera.zoom -= 0.015f;
		if ( playerCamera.zoom < 0.2f ) {
			playerCamera.zoom = 0.2f;
		}
		return;
	}
}

void vc::GameMode::HandleKeyboardEvent( int button, bool buttonUp ) {
	if ( buttonUp ) {
		return;
	}

	switch ( button ) {
		default:
			break;

		case ALLEGRO_KEY_F5: {
			// Quick save check
			SaveGame( "Quick.save" );
			break;
		}

		case ALLEGRO_KEY_F6: {
			// Quick load check
			RestoreGame( "Quick.save" );
			break;
		}

		case ALLEGRO_KEY_ESCAPE:
		case ALLEGRO_KEY_PAUSE: {
			// TODO: pause/unpause sounds
			if ( gameState == GameState::PAUSED ) {
				gameState = GameState::ACTIVE;
				break;
			}

			gameState = GameState::PAUSED;
			break;
		}
	}
}
