/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "GameMode.h"
#include "GUIPanel.h"
#include "SimGame.h"
#include "agent.h"

vc::GameMode::GameMode() {
	// Cache all the data we're going to use...

	vc::GetApp()->CacheFont( "data/fonts/ps2p/PressStart2P.ttf", FONT_SIZE_LARGE );
	vc::GetApp()->CacheFont( "data/fonts/ps2p/PressStart2P.ttf", FONT_SIZE_SMALL );
	vc::GetApp()->CacheFont( "data/fonts/ps2p/PressStart2P.ttf", 32 );
	vc::GetApp()->CacheFont( "data/fonts/ps2p/PressStart2P.ttf", FONT_SIZE_MEDIUM );

	vc::GetApp()->CacheSample( "data/sounds/00.wav" );
	vc::GetApp()->CacheSample( "data/sounds/01.wav" );
	vc::GetApp()->CacheSample( "data/sounds/02.wav" );
	vc::GetApp()->CacheSample( "data/sounds/03.wav" );
	vc::GetApp()->CacheSample( "data/sounds/04.wav" );
	vc::GetApp()->CacheSample( "data/sounds/05.wav" );
	vc::GetApp()->CacheSample( "data/sounds/06.wav" );

	terrainSheet = new SpriteSheet( "sprites:terrain.sdf", vc::GetApp()->CacheImage( "sprites:terrain.gfx" ) );
	interfaceSheet = new SpriteSheet( "sprites:interface.sdf", vc::GetApp()->CacheImage( "sprites:interface.gfx" ) );

	// Register agents
	AgentFactory::Get()->RegisterScripts();

	// Now create the base GUI panel
	uiPanelPtr = new GUIPanel( nullptr, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );
}

vc::GameMode::~GameMode() = default;

void vc::GameMode::Tick() {
	if ( curGameState == GameState::PAUSED ) {
		return;
	}

	AgentFactory::Get()->Tick();
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
