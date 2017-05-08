// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "../Shared.h"

#include "game.h"

#include "CreatureBrain.h"
#include "objects/World.h"
#include "../engine/Sprite.hpp"

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
        //HAND_STATE_OPEN_UNDER,
        HAND_STATE_OPEN_BACK,
        HAND_STATE_CLOSED,
        HAND_STATE_FLAT,
        HAND_STATE_FLAT_BACK,
        //HAND_STATE_PET,
        //HAND_STATE_PICKUP,

        HAND_STATE_NONE
    };

    Hand() : Sprite(engine::LoadImage("cursor/point")), target_angle(0) {
        // todo, set this in m_LocalPosition of cursor on creation.
        m_Origin.Set(0, 0);

        hand_state[HAND_STATE_DEFAULT]      = GetBitmap();
        hand_state[HAND_STATE_CLOSED]       = engine::LoadImage("cursor/closed");
        hand_state[HAND_STATE_FLAT]         = engine::LoadImage("cursor/flat1");
        hand_state[HAND_STATE_FLAT_BACK]    = engine::LoadImage("cursor/flat0");
        hand_state[HAND_STATE_OPEN]         = engine::LoadImage("cursor/open");
        hand_state[HAND_STATE_OPEN_BACK]    = engine::LoadImage("cursor/open2");
    }

    void SetState(HandState state) {
        if(state == HAND_STATE_NONE) {
            return;
        }
        SetBitmap(hand_state[state]);
    }

    void Simulate() {
        m_LocalPosition.Set(
                engine_vars.mouse_state.x,
                engine_vars.mouse_state.y
        );

        move_time++;
        if (engine_vars.mouse_state.buttons) {
          //  m_LocalPosition.y += 5;
            if(target_angle != 0.2f) {
                move_time = 0;
            }
            target_angle = 0.2f;
        } else {
            if(target_angle != 0) {
                move_time = 0;
            }
            target_angle = 0;
        }

        if(m_Angle != target_angle) {
            m_Angle = plCosineInterpolate(m_Angle, target_angle, move_time / 6);
        }
    }

protected:
private:
    float move_time, target_angle;

    ALLEGRO_BITMAP *hand_state[HAND_STATE_NONE];
};

Hand *game_userhand = nullptr;

// Menu (includes HUD etc.)

enum GameMenuState {
    GAME_MENU_START,    // "Press start to begin the game!"
    GAME_MENU_MAIN,     // Main Menu
    GAME_MENU_NEW,      // New Game
    GAME_MENU_LOAD,     // Load Game
    GAME_MENU_OPTIONS,  // Options
    GAME_MENU_QUIT,     // Quit

    GAME_MENU_PAUSED,   // Paused screen

    GAME_MENU_DEFAULT   // in-game?
} GameMenuState;

// Everything Else

//ALLEGRO_BITMAP *background0, *background1, *background2;

void InitializeGame() {
    memset(&game, 0, sizeof(GameVars));

    game.state = GAME_STATE_MENU;
    game.menu_state = GAME_MENU_START;

    game.font_title = engine::LoadFont("pacifico/Pacifico-Bold", 80);
    game.font_small = engine::LoadFont("league_gothic/LeagueGothic-Regular", 21);
    game.font_gothic_medium = engine::LoadFont("league_gothic/LeagueGothic-Regular", 32);
    game.font_chunk = engine::LoadFont("chunk/Chunk", 24);

    game.menu_earth = engine::LoadImage("environment/objects/earth");

    // Editor Icons
    game.entity_icon = engine::LoadImage("default_entity");

    //background0 = engine::LoadImage("placeholder/skill-desc_0000_foreground");
    //background1 = engine::LoadImage("placeholder/skill-desc_0001_buildings");
    //background2 = engine::LoadImage("placeholder/skill-desc_0002_far-buildings");

    game_userhand = new Hand();
    World::GetInstance();
}

void draw_menu() {

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
            const int grad_alpha = 200;
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
            //DrawBitmap(background2, 0 - game.camera_x / 3, 128 - game.camera_y, 1088, 416);
            //DrawBitmap(background1, 0 - game.camera_x / 2, 128 - game.camera_y, 1088, 416);
            //DrawBitmap(background0, 0 - game.camera_x, 128 - game.camera_y, 1088, 416);

            DrawVerticalGradientRectangle(
                    0, DISPLAY_HEIGHT - 128,
                    DISPLAY_WIDTH, 128,
                    al_map_rgba(0, 0, 0, 0),
                    al_map_rgba(0, 0, 0, 72)
            );

            // Time counter
            DrawString(game.font_small, 20, 420, al_map_rgb(255, 255, 255), World::GetInstance()->GetDayString());
            char timestr[10] = {0};
            snprintf(timestr, 10, "%02u:%02u",
                     World::GetInstance()->GetHour(),
                     World::GetInstance()->GetMinute()); // Removed the second timer here...
            DrawString(game.font_small, 20, 440, al_map_rgb(255, 255, 255), timestr);
            break;
        }
    }

    if(game.state == GAME_STATE_MENU) {
        DrawCenteredString(
                game.font_title,
                DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4,
                /*230, 80, 20*/ al_map_rgb(255, 255, 255),
                "Virtual Critters Inc."
        );

#if 1
        DrawCenteredString(
                game.font_small,
                DISPLAY_WIDTH / 2,
                DISPLAY_HEIGHT - 42,
                al_map_rgb(255, 255, 255),
                "Virtual Critters Inc., Copyright (C) 2017 Mark Elsworth Sowden"
        );
#endif
    }
}

void GameDisplayFrame() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    World::GetInstance()->Draw();

    draw_menu();

    game_userhand->Draw();
}

#define CAMERA_MAXSPEED     10
#define CAMERA_ACCELERATION 0.005f

void GameTimerFrame() {
    game_userhand->Simulate();

    switch (game.state) {

        default:
        case GAME_STATE_DEFAULT: {
            static PLVector2D camera_movement(0, 0);

            game.camera_x += camera_movement.x;
            game.camera_y += camera_movement.y;

            if(engine_vars.key_status[ALLEGRO_KEY_LEFT]) {

                if(camera_movement.x > 1024) {
                    camera_movement.x = 1024;
                }

                if(camera_movement.x > -CAMERA_MAXSPEED) {
                    camera_movement.x -= CAMERA_ACCELERATION;
                }

                if (!game.camera_x) {
                    game.camera_x = 0; camera_movement.x = 0;
                }
            } else if(engine_vars.key_status[ALLEGRO_KEY_RIGHT]) {

                if(camera_movement.x < -1024) {
                    camera_movement.x = -1024;
                }

                if(camera_movement.x < CAMERA_MAXSPEED) {
                    camera_movement.x += CAMERA_ACCELERATION;
                }

                if (game.camera_x > World::GetInstance()->GetHeight() - DISPLAY_HEIGHT) {
                    game.camera_x = World::GetInstance()->GetHeight(); camera_movement.x = 0;
                }
            } else {
                if(camera_movement.x > 0) {
                    camera_movement.x -= CAMERA_ACCELERATION * 10.f;
                } else if(camera_movement.x < 0) {
                    camera_movement.x += CAMERA_ACCELERATION * 10.f;
                }
            }

            if(engine_vars.key_status[ALLEGRO_KEY_DOWN]) {

                if(camera_movement.y < -512) {
                    camera_movement.y = 0;
                }

                if(camera_movement.y < CAMERA_MAXSPEED) {
                    camera_movement.y += CAMERA_ACCELERATION;
                }

                if (game.camera_y > World::GetInstance()->GetWidth() - DISPLAY_WIDTH) {
                    game.camera_y = World::GetInstance()->GetWidth(); camera_movement.y = 0;
                }
            } else if(engine_vars.key_status[ALLEGRO_KEY_UP]) {

                if(camera_movement.y > 2048) {
                    camera_movement.y = 2048;
                }

                if(camera_movement.y > -CAMERA_MAXSPEED) {
                    camera_movement.y -= CAMERA_ACCELERATION;
                }

                if (!game.camera_y) {
                    game.camera_y = 0; camera_movement.y = 0;
                }
            } else {
                if(camera_movement.y > 0) {
                    camera_movement.y -= CAMERA_ACCELERATION * 10.f;
                } else if(camera_movement.y < 0) {
                    camera_movement.y += CAMERA_ACCELERATION * 10.f;
                }
            }

            World::GetInstance()->Simulate();
            break;
        }

        case GAME_STATE_MENU: {
            World::GetInstance()->Simulate();
            break;
        }

        case GAME_STATE_PAUSED: break;
    }
}

void MouseEvent(void) {

}

void KeyboardEvent(int code, bool keyup) {
    if(keyup) {
        return;
    }

    switch(code) {
        default: break;

        case ALLEGRO_KEY_PAUSE: {
            if(game.state == GAME_STATE_PAUSED) {
                game.state = game.old_state;
                game.menu_state = game.menu_old_state;
                break;
            }

            // todo, play pause sound.

            game.old_state = game.state;
            game.menu_old_state = game.menu_state;

            game.state = GAME_STATE_PAUSED;
            game.menu_state = GAME_MENU_PAUSED;
            break;
        }

        case ALLEGRO_KEY_ESCAPE:
            if((game.state == GAME_STATE_MENU) && (game.old_state == GAME_STATE_DEFAULT)) {
                game.state = GAME_STATE_DEFAULT;
                game.menu_state = GAME_MENU_DEFAULT;
                break;
            }

            // todo, play menu sound.

            game.old_state = game.state;
            game.menu_old_state = game.menu_state;

            game.state = GAME_STATE_MENU;
            game.menu_state = GAME_MENU_MAIN;
            break;
    }
}

void ShutdownGame() {}

