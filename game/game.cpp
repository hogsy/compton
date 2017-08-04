// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../shared.h"

#include "game.h"

#include "objects/creature/creature.h"
#include "objects/object_world.h"

#include "../engine/sprite.h"

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
    enum {
        HAND_STATE_POINT,
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

    enum {
        HAND_MODE_DEFAULT,
        HAND_MODE_DRAG,
    };

    Hand() : Sprite(engine::LoadImage("cursor/point")) {
        // todo, set this in position of cursor on creation.
        origin_.Set(0, 0);
        transition_delay_ = 0;

        hand_state_[HAND_STATE_POINT]       = GetBitmap();
        hand_state_[HAND_STATE_CLOSED]      = engine::LoadImage("cursor/closed");
        hand_state_[HAND_STATE_FLAT]        = engine::LoadImage("cursor/flat1");
        hand_state_[HAND_STATE_FLAT_BACK]   = engine::LoadImage("cursor/flat0");
        hand_state_[HAND_STATE_OPEN]        = engine::LoadImage("cursor/open");
        hand_state_[HAND_STATE_OPEN_BACK]   = engine::LoadImage("cursor/open2");

        current_state_ = old_state_ = HAND_STATE_POINT;
        current_mode_ = old_mode_ = HAND_MODE_DEFAULT;
    }

    void SetState(unsigned int state) {
        old_state_ = current_state_;
        current_state_ = state;
        SetBitmap(hand_state_[state]);
    }

    void SetMode(unsigned int mode) {
        old_mode_ = current_mode_;
        current_mode_ = mode;
    }

    void Draw() override {
        PLVector2D oldpos = position_;
        if(current_state_ == HAND_STATE_CLOSED && current_mode_ == HAND_MODE_DRAG) {
            position_.x -= game.camera_x;
            position_.y -= game.camera_y;
        }
        Sprite::Draw();
        position_ = oldpos;
    }

    void Simulate() {
        move_time_++;
        if(transition_delay_ != 0) {
            transition_delay_--;
        }

        switch(current_mode_) {
            case HAND_MODE_DEFAULT: {
                position_.Set(
                        engine_vars.mouse_state.x,
                        engine_vars.mouse_state.y
                );

                if ((engine_vars.mouse_state.buttons & BUTTON_LMOUSE) != 0) {
                    position_.y += 5;
                    if(target_angle_ != -0.2f) {
                        move_time_ = 0;
                    }
                    target_angle_ = -0.2f;
                } else {
                    if(target_angle_ != 0) {
                        move_time_ = 0;
                    }
                    target_angle_ = 0;
                }

                if(current_state_ != HAND_STATE_POINT) {
                    if(transition_delay_ == 0) {
                        SetState(HAND_STATE_POINT);
                    }
                    break;
                }

                if((engine_vars.mouse_state.buttons & BUTTON_MMOUSE) != 0) {
                    SetState(HAND_STATE_OPEN);
                    SetMode(HAND_MODE_DRAG);
                    transition_delay_ = 9;
                }

                break;
            }

            case HAND_MODE_DRAG: { // drag the camera around
                if((engine_vars.mouse_state.buttons & BUTTON_MMOUSE) == 0) {
                    SetState(HAND_STATE_OPEN);
                    SetMode(HAND_MODE_DEFAULT);
                    transition_delay_ = 9;
                }

                if(current_state_ != HAND_STATE_CLOSED) {
                    position_.Set(
                            engine_vars.mouse_state.x,
                            engine_vars.mouse_state.y
                    );

                    if(target_angle_ != -1.5f) {
                        move_time_ = 0;
                    }
                    target_angle_ = -1.5f;

                    if(transition_delay_ == 0) {
                        SetState(HAND_STATE_CLOSED);
                        old_position_ = position_;
                    }
                    break;
                }

                int nxpos = engine_vars.mouse_state.x - static_cast<int>(old_position_.x);
                int nypos = engine_vars.mouse_state.y - static_cast<int>(old_position_.y);
#if 0
                float dir = (plCreateVector2D(nxpos, nypos) -
                        plCreateVector2D(position.x, position_.y)).Length() * 180 / PL_PI;

                float angle = -1.5f + dir;
                if(m_TargetAngle != angle) {
                    m_MoveTime = 0;
                }
                m_TargetAngle = angle;
#endif
                game.camera_x += (nxpos / 100);
                game.camera_y += (nypos / 100);

                //position_ = old_position_;
                break;
            }

            default: break;
        }

        if(angle != target_angle_) {
            angle = plCosineInterpolate(angle, target_angle_, move_time_ / 6);
        }
    }

protected:
private:
    float move_time_{}, transition_delay_, target_angle_{};

    unsigned int current_state_, old_state_;
    ALLEGRO_BITMAP *hand_state_[HAND_STATE_NONE]{};

    PLVector2D old_position_;

    unsigned int current_mode_, old_mode_;
};

Hand *game_userhand = nullptr;

// Menu (includes HUD etc.)



// Everything Else

//ALLEGRO_BITMAP *background0, *background1, *background2;

void InitializeGame() {
    std::printf("\nInitializing game sub-system...\n");

    memset(&game, 0, sizeof(GameVars));

    game.state = GAME_STATE_MENU;
    game.menu_state = GAME_MENU_START;

    plGetUserName(game.profile);
    if(game.profile[0] == '\0') {
        std::printf("Failed to get base profile, using default...\n");
        std::snprintf(game.profile, sizeof(game.profile), "default");
    }

    std::printf("Current profile %s\n", game.profile);

    game.font_title             = engine::LoadFont("steps-mono-master/fonts/Steps-Mono-Thin", 50);
    game.font_small             = engine::LoadFont("league_gothic/LeagueGothic-Regular", 21);
    game.font_gothic_medium     = engine::LoadFont("league_gothic/LeagueGothic-Regular", 32);
    game.font_chunk             = engine::LoadFont("chunk/Chunk", 24);

    // Menu
    game.menu_earth = engine::LoadImage("environment/objects/earth");

    // Editor Icons
    game.entity_icon = engine::LoadImage("default_entity");

    //background0 = engine::LoadImage("placeholder/skill-desc_0000_foreground");
    //background1 = engine::LoadImage("placeholder/skill-desc_0001_buildings");
    //background2 = engine::LoadImage("placeholder/skill-desc_0002_far-buildings");

    game_userhand = new Hand();
    World::GetInstance();
}

void DrawMenu() {

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
            srand(1);
            for(unsigned int i = 0; i < 1024; ++i) {
                al_draw_pixel(
                std::rand()%DISPLAY_WIDTH,
                std::rand()%DISPLAY_HEIGHT,
                al_map_rgb(255, 255, 255));
            }
            srand((unsigned)time(nullptr));

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

#define HANDYBLEND (-(std::cos(static_cast<float>(engine_vars.counter / 50)) * 0.8f))
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

            DrawString(game.font_small, 20, 20, al_map_rgb(255, 255, 255), World::GetInstance()->GetName().c_str());

            // Time counter
            char monthstr[256] = {0};
            std::snprintf(monthstr, sizeof(monthstr), "%s, on the day of %s",
                     World::GetInstance()->GetMonthString(),
                     World::GetInstance()->GetDayString()
            );
            DrawString(game.font_small, 20, 420, al_map_rgb(255, 255, 255), monthstr);

            std::snprintf(monthstr, sizeof(monthstr), "%04u/%02u/%02u",
                     World::GetInstance()->GetYear(),
                     World::GetInstance()->GetMonth(),
                     World::GetInstance()->GetDay()
            );
            DrawString(game.font_small, 540, 420, al_map_rgb(255, 255, 255), monthstr);

            char timestr[10] = {0};
            std::snprintf(timestr, 10, "%02u:%02u",
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
                "VIRTUAL CRITTERS INC."
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

    DrawMenu();

    game_userhand->Draw();
}

#define CAMERA_MAXSPEED     10
#define CAMERA_ACCELERATION 0.05f

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

                if (game.camera_x <= 0) {
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

                if (game.camera_y <= 0) {
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

void MouseEvent() {

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

