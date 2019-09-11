// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

typedef struct GameVars {
    unsigned int state, old_state;
    unsigned int menu_state, menu_old_state;

    ALLEGRO_BITMAP *menu_earth;

    // Editor Icons
    ALLEGRO_BITMAP *entity_icon;

    ALLEGRO_FONT *font_title;
    ALLEGRO_FONT *font_small, *font_gothic_medium;
    ALLEGRO_FONT *font_chunk;

    // sounds...
    ALLEGRO_SAMPLE *sample_jump;
    ALLEGRO_SAMPLE *sample_throw;
    ALLEGRO_SAMPLE *sample_land;
    ALLEGRO_SAMPLE *sample_pickup;
    ALLEGRO_SAMPLE *sample_charge;

    char profile[PL_SYSTEM_MAX_USERNAME];

    float camera_x, camera_y;

    bool is_grabbing;
} GameVars;

extern GameVars game;

///////////////////////////////////////////

enum {
    GAME_MENU_START,    // "Press start to begin the game!"
    GAME_MENU_MAIN,     // Main Menu
    GAME_MENU_NEW,      // New Game
    GAME_MENU_LOAD,     // Load Game
    GAME_MENU_OPTIONS,  // Options
    GAME_MENU_QUIT,     // Quit

    GAME_MENU_PAUSED,   // Paused screen

    GAME_MENU_DEFAULT   // in-game?
};

///////////////////////////////////////////

#define GAME_PHYSICS_DEFAULT  0.085f

#include "../engine/entity.h"
#include "../engine/entity_factory.h"

