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
	GAME_MENU_MAIN,	  // Main Menu
	GAME_MENU_NEW,	  // New Game
	GAME_MENU_LOAD,	  // Load Game
	GAME_MENU_OPTIONS,// Options
	GAME_MENU_QUIT,	  // Quit

	GAME_MENU_PAUSED,// Paused screen

	GAME_MENU_DEFAULT// in-game?
};

///////////////////////////////////////////

#define GAME_PHYSICS_DEFAULT 0.085f
