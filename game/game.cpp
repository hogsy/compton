// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "../Shared.h"

#include "game.h"

#include "CreatureBrain.h"
#include "WorldManager.h"
#include "Sprite.hpp"

/*	Game logic and other crap goes here!	*/

enum GameState {
    GAME_STATE_DEFAULT,
    GAME_STATE_MENU,
    GAME_STATE_PAUSED,
} GameState;

GameVars game;

// Text Prompt

class SubtitleSentence {
public:
protected:
private:
    std::string _sentence;
};

class SubtitleSystem {
public:
    SubtitleSystem() {}

    ~SubtitleSystem() {};

    void Draw() {

    }

    void Simulate() {

    }

protected:
private:
};

// Hand



class Hand : public Sprite {
public:
    enum HandState {
        HAND_STATE_DEFAULT,    // Point
        HAND_STATE_OPEN,

        HAND_STATE_NONE
    };

    Hand() : Sprite(engine::LoadImage("cursor/point")) {
        // todo, set this in position of cursor on creation.
        origin.Set(0, 0);

        hand_state[HAND_STATE_DEFAULT] = GetSprite();
    }

    void SetState(HandState state) {
        if(state == HAND_STATE_NONE) {
            return;
        }
        SetSprite(hand_state[state]);
    }

    void Simulate() {
        position.Set(
                engine_vars.mouse_state.x,
                engine_vars.mouse_state.y
        );

        angle = 0;
        if (engine_vars.mouse_state.buttons) {
            position.y += 5;
            angle -= 0.2f;
        }
    }

protected:
private:
    ALLEGRO_BITMAP *hand_state[HAND_STATE_NONE];
};

Hand *game_userhand = nullptr;

// Menu (includes HUD etc.)

enum GameMenuState {
    GAME_MENU_START,        // "Press start to begin the game!"
    GAME_MENU_MAIN,            // Main Menu
    GAME_MENU_NEW,        // New Game
    GAME_MENU_LOAD,        // Load Game
    GAME_MENU_OPTIONS,    // Options
    GAME_MENU_QUIT,        // Quit

    GAME_MENU_PAUSED,   // Paused screen

    GAME_MENU_DEFAULT            // in-game?
} GameMenuState;

void DrawGameMenu() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    game_worldmanager->Draw();

    switch (game.menu_state) {
        default:break;

        case GAME_MENU_PAUSED: {
            DrawCenteredString(
                    game.font_title,
                    DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4,
                    /*230, 80, 20*/ al_map_rgb(255, 255, 255),
                    "... PAUSED ..."
            );

            break;
        }

        case GAME_MENU_START: {
            // todo, interpolate this up at the beginning of this menu state.
            al_draw_rotated_bitmap(
                    game.menu_earth,
                    al_get_bitmap_width(game.menu_earth) / 2,
                    al_get_bitmap_height(game.menu_earth) / 2,
                    (DISPLAY_WIDTH / 2),
                    850,
                    (float) (engine_vars.counter / 1000),
                    0
            );

            const int grad_height = 128;
            const int grad_alpha = 62;
            DrawVerticalGradientRectangle(
                    0, 0,
                    DISPLAY_WIDTH, grad_height,
                    al_map_rgba(0, 0, 0, grad_alpha),
                    al_map_rgba(0, 0, 0, 0)
            );
            DrawVerticalGradientRectangle(
                    0, DISPLAY_HEIGHT - grad_height,
                    DISPLAY_WIDTH, grad_height,
                    al_map_rgba(0, 0, 0, 0),
                    al_map_rgba(0, 0, 0, grad_alpha)
            );
            DrawVerticalGradientRectangle(
                    0, 153,
                    DISPLAY_WIDTH, 80,
                    al_map_rgba(0, 0, 0, 150),
                    al_map_rgba(0, 0, 0, 150)
            );
            DrawCenteredString(
                    game.font_title,
                    DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4,
                    /*230, 80, 20*/ al_map_rgb(255, 255, 255),
                    "Virtual Critters Inc."
            );
#if 0
            DrawCenteredString(
                    game.font_small,
                    DISPLAY_WIDTH / 2,
                    DISPLAY_HEIGHT - 42,
                    al_map_rgb(255, 255, 255),
                    "Virtual Critters Inc., Copyright (C) 2016 Mark Elsworth Sowden"
            );
#endif

#define HANDYBLEND -(std::cos((float)engine_vars.counter / 50) * 0.8f)
            DrawCenteredString(
                    game.font_gothic_medium,
                    DISPLAY_WIDTH / 2, 250,
                    // We have to fade ALL the channels here, otherwise
                    // we don't fade properly (yay Allegro).
                    al_map_rgba_f(
                            HANDYBLEND,
                            HANDYBLEND,
                            HANDYBLEND,
                            HANDYBLEND
                    ),
                    "PRESS ANY KEY TO START"
            );

            break;
        }

        case GAME_MENU_MAIN: {
            break;
        }

        case GAME_MENU_NEW: {
            break;
        }

        case GAME_MENU_LOAD: {
            break;
        }

        case GAME_MENU_OPTIONS: {
            break;
        }

        case GAME_MENU_QUIT: {
            break;
        }

        case GAME_MENU_DEFAULT: {
            // Time counter
            DrawString(game.font_small, 20, 420, al_map_rgb(255, 255, 255), game_worldmanager->GetDayString());
            char timestr[10] = {0};
            snprintf(timestr, 10, "%02u:%02u",
                     game_worldmanager->GetHour(),
                     game_worldmanager->GetMinute()); // Removed the second timer here...
            DrawString(game.font_small, 20, 440, al_map_rgb(255, 255, 255), timestr);
            break;
        }
    }

    game_userhand->Draw();
}

// Everything Else

void InitializeGame() {
    memset(&game, 0, sizeof(GameVars));

    game.state = GAME_STATE_MENU;
    game.menu_state = GAME_MENU_START;

    game.font_title = engine::LoadFont("pacifico/Pacifico-Bold", 80);
    game.font_small = engine::LoadFont("league_gothic/LeagueGothic-Regular", 21);
    game.font_gothic_medium = engine::LoadFont("league_gothic/LeagueGothic-Regular", 32);
    game.font_chunk = engine::LoadFont("chunk/Chunk", 24);

    game.menu_earth = engine::LoadImage("environment/objects/earth");

    game_userhand = new Hand();
    game_worldmanager = new WorldManager();
}

void GameDisplayFrame() {
    DrawGameMenu();
}

void GameTimerFrame() {
    game_userhand->Simulate();

    switch (game.state) {
        default:
        case GAME_STATE_MENU:
            game_worldmanager->Simulate();
            break;

        case GAME_STATE_PAUSED: break;
    }
}

void ShutdownGame() {}

// Input...

void GameDefaultKeyboardInput(int code, bool keyup) {
    if(keyup) {
        return;
    }

    switch(code) {
        default: break;

        case ALLEGRO_KEY_PAUSE:
            // todo, play pause sound.
            game.oldstate = game.state;
            game.state = GAME_STATE_PAUSED;
            game.menu_state = GAME_MENU_PAUSED;
            break;

        case ALLEGRO_KEY_UP:
            // todo, move camera up.
        case ALLEGRO_KEY_DOWN:
            // todo, move camera down.
        case ALLEGRO_KEY_LEFT:
            // todo, move camera left.
        case ALLEGRO_KEY_RIGHT:
            // todo, move camera right.
            break;

        case ALLEGRO_KEY_ESCAPE:
            // todo, play menu sound.
            game.oldstate = game.state;
            game.state = GAME_STATE_MENU;
            game.menu_state = GAME_MENU_MAIN;
            break;
    }
}

void GamePauseKeyboardInput(int code, bool keyup) {
    if(keyup) {
        return;
    }

    switch(code) {
        default:break;

        case ALLEGRO_KEY_PAUSE:
            // todo, play unpause sound.
            game.state = game.oldstate;
            break;
    }
}

void GameMenuKeyboardInput(int code, bool keyup) {
    if(keyup) {
        return;
    }

    if(code == ALLEGRO_KEY_PAUSE) {
        game.oldstate = game.state;
        game.state = GAME_STATE_PAUSED;
        return;
    }

    switch(game.menu_state) {
        default:break;

        case GAME_MENU_START:
            // todo, play start sound.
            game.menu_state = GAME_MENU_MAIN;
            game.state = GAME_STATE_DEFAULT;
            break;

        case GAME_MENU_MAIN:
            switch(code) {
                default:break;

                case ALLEGRO_KEY_ESCAPE:
                    // todo, play menu sound.
                    if(game.oldstate != GAME_STATE_MENU) {
                        game.state = game.oldstate;
                    } else {
                        game.state = GAME_STATE_DEFAULT;
                    }
                    game.menu_state = GAME_MENU_DEFAULT;
                    break;
            }
    }
}

void GameKeyboardInput(int code, bool keyup) {
    switch(game.state) {
        default: break;

        case GAME_STATE_DEFAULT:
            GameDefaultKeyboardInput(code, keyup);
            break;

        case GAME_STATE_MENU:
            GameMenuKeyboardInput(code, keyup);
            break;

        case GAME_STATE_PAUSED:
            GamePauseKeyboardInput(code, keyup);
            break;
    }
}

