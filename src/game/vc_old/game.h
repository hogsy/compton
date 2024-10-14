// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

typedef struct GameVars
{
	unsigned int state, old_state;
	unsigned int menu_state, menu_old_state;

	ALLEGRO_FONT *font_title;
	ALLEGRO_FONT *font_small;

	// sounds...
	ALLEGRO_SAMPLE *sample_land;
	ALLEGRO_SAMPLE *sample_charge;

	char profile[ PL_SYSTEM_MAX_USERNAME ];

	float camera_x, camera_y;
} GameVars;

extern GameVars game;

///////////////////////////////////////////

enum
{
	GAME_MENU_START,  // "Press start to begin the game!"
	GAME_MENU_MAIN,   // Main Menu
	GAME_MENU_NEW,    // New Game
	GAME_MENU_LOAD,   // Load Game
	GAME_MENU_OPTIONS,// Options
	GAME_MENU_QUIT,   // Quit

	GAME_MENU_PAUSED,// Paused screen

	GAME_MENU_DEFAULT// in-game?
};

///////////////////////////////////////////

#define GAME_PHYSICS_DEFAULT 0.085f
