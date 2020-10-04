/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "GameMode.h"
#include "GUI/GUIButton.h"
#include "GUI/GUICursor.h"
#include "GUI/GUIPanel.h"
#include "Terrain.h"
#include "EntityManager.h"
#include "Serializer.h"
#include "agent.h"

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

	terrainSheet = new SpriteSheet( "sheets/terrain.sdf", vc::GetApp()->CacheImage( "sheets/terrain.png" ) );
	terrainPtr = new Terrain();

	entityManager = new EntityManager();

	NewGame( "Fudge" );
}

vc::GameMode::~GameMode() = default;

void vc::GameMode::Tick() {
	// Always tick UI, because we still want to access it even if paused
	uiBasePanelPtr->Tick();

	if ( curGameState == GameState::PAUSED ) {
		return;
	}

	terrainPtr->Tick();
	entityManager->Tick();
}

void vc::GameMode::Draw() {
	terrainPtr->Draw();
	entityManager->Draw();
	uiBasePanelPtr->Draw();
}

void vc::GameMode::NewGame( const char *path ) {

}

void vc::GameMode::SaveGame( const char *path ) {
	Serializer serializer( path, Serializer::Mode::WRITE );
	terrainPtr->Serialize( &serializer );
	entityManager->SerializeEntities( &serializer );
}

void vc::GameMode::RestoreGame( const char *path ) {
	Serializer serializer( path, Serializer::Mode::READ );
	terrainPtr->Deserialize( &serializer );
	entityManager->DeserializeEntities( &serializer );
}

void vc::GameMode::HandleMouseEvent( int x, int y, int button, bool buttonUp ) {
}

void vc::GameMode::HandleKeyboardEvent( int button, bool buttonUp ) {
	if ( buttonUp ) {
		return;
	}

	switch ( button ) {
		default:
			break;

		case ALLEGRO_KEY_ENTER: {
			AgentFactory::Get()->Clear();

			static unsigned int c = 1;
			for ( unsigned int i = 0; i < c; ++i ) {
				Agent *agent = AgentFactory::Get()->Create( "Ball" );
				agent->SetPosition( { static_cast< float >( rand() % DISPLAY_WIDTH ),
				                      static_cast< float >( rand() % 128 ) } );
			}
			Print( "%d actors\n", AgentFactory::Get()->GetNumOfAgents() );
			c *= 2;
			break;
		}

		case ALLEGRO_KEY_ESCAPE:
		case ALLEGRO_KEY_PAUSE: {
			// TODO: pause/unpause sounds
			if ( curGameState == GameState::PAUSED ) {
				curGameState = GameState::ACTIVE;
				break;
			}

			curGameState = GameState::PAUSED;
			break;
		}
	}
}
