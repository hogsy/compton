// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

typedef struct GameVars {
    unsigned int state, oldstate;
    unsigned int menu_state;

    ALLEGRO_BITMAP *menu_earth;

    ALLEGRO_FONT *font_title;
    ALLEGRO_FONT *font_small, *font_gothic_medium;
    ALLEGRO_FONT *font_chunk;
} GameVars;

extern GameVars game;

///////////////////////////////////////////

