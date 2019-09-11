// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../shared.h"
#include "game.h"
#include "agent.h"
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

#if 0
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

        state_ = old_state_ = HAND_STATE_POINT;
        current_mode_ = old_mode_ = HAND_MODE_DEFAULT;
    }

    void SetState(unsigned int state) {
        old_state_ = state_;
        state_ = state;
        SetBitmap(hand_state_[state]);
    }

    void SetMode(unsigned int mode) {
        old_mode_ = current_mode_;
        current_mode_ = mode;
    }

    void Draw() override {
        PLVector2D oldpos = position_;
        if(state_ == HAND_STATE_CLOSED && current_mode_ == HAND_MODE_DRAG) {
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

                if(state_ != HAND_STATE_POINT) {
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

                if(state_ != HAND_STATE_CLOSED) {
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

    unsigned int state_, old_state_;
    ALLEGRO_BITMAP *hand_state_[HAND_STATE_NONE]{};

    PLVector2D old_position_;

    unsigned int current_mode_, old_mode_;
};
#endif

// Menu (includes HUD etc.)



// Everything Else

//ALLEGRO_BITMAP *background0, *background1, *background2;

ALLEGRO_BITMAP* status_sprite;

void InitializeGame() {

  memset(&game, 0, sizeof(GameVars));

  game.state = GAME_STATE_DEFAULT;
  game.menu_state = GAME_MENU_DEFAULT;

  game.font_title = engine::LoadFont("ps2p/PressStart2P", 50);
  game.font_small = engine::LoadFont("ps2p/PressStart2P", 8);
  game.font_gothic_medium = engine::LoadFont("ps2p/PressStart2P", 32);
  game.font_chunk = engine::LoadFont("ps2p/PressStart2P", 24);

  game.sample_jump = engine::LoadSample("00.wav");
  game.sample_land = engine::LoadSample("03.wav");
  game.sample_charge = engine::LoadSample("04.wav");
  game.sample_pickup = engine::LoadSample("05.wav");
  game.sample_throw = engine::LoadSample("06.wav");

  status_sprite = engine::LoadImage("sprites");

  game.is_grabbing = false;

  World::GetInstance();

  creature = new Creature();
  toy = new CreatureToy();
  drink = new CreatureDrink();
}

void DrawMenu() {

  //DrawBitmap(background2, 0 - game.camera_x / 3, 128 - game.camera_y, 1088, 416);
  //DrawBitmap(background1, 0 - game.camera_x / 2, 128 - game.camera_y, 1088, 416);
  //DrawBitmap(background0, 0 - game.camera_x, 128 - game.camera_y, 1088, 416);

  // Time counter
#if 0
  static float x_scroll = 0;
  static bool scroll_rotation = true;
  if(scroll_rotation) {
      x_scroll += 0.15f;
      if(x_scroll > 3) {
          scroll_rotation = false;
      }
  } else {
      x_scroll -= 0.15f;
      if (x_scroll < -18) {
          scroll_rotation = true;
      }
  }
  char monthstr[20] = {0};
  snprintf(monthstr, sizeof(monthstr), "%04u/%02u/%02u",
           World::GetInstance()->GetYear(),
           World::GetInstance()->GetMonth(),
           World::GetInstance()->GetDay()
  );
  DrawString(game.font_small, static_cast<int>(x_scroll) + 1, 11, al_map_rgb(0, 0, 0), monthstr);
  DrawString(game.font_small, static_cast<int>(x_scroll), 10, al_map_rgb(255, 255, 255), monthstr);
#else
  std::string cur_day = "DAY " + std::to_string(World::GetInstance()->GetTotalDays() + 1);
  DrawString(game.font_small, 3, 11, al_map_rgb(0, 0, 0), cur_day.c_str());
  DrawString(game.font_small, 2, 10, al_map_rgb(255, 255, 255), cur_day.c_str());
#endif

  char timestr[10] = {0};
  static char blink = ':', blink_delay = 0;
  if (blink_delay == 50) {
    if (blink == ':') {
      blink = ' ';
    } else {
      blink = ':';
    }
    blink_delay = 0;
  }
  snprintf(timestr, 10, "%02u%c%02u",
           World::GetInstance()->GetHour(), blink,
           World::GetInstance()->GetMinute()); // Removed the second timer here...
  DrawString(game.font_small, 3, 3, al_map_rgb(0, 0, 0), timestr);
  DrawString(game.font_small, 2, 2, al_map_rgb(255, 255, 255), timestr);
  blink_delay++;

  switch (creature->GetState()) {
    default:
    case Creature::EMO_HAPPINESS:al_draw_bitmap_region(status_sprite, 0, 64, 8, 8, 2, 55, 0);
      break;

    case Creature::EMO_INDIFFERENT:al_draw_bitmap_region(status_sprite, 8, 64, 8, 8, 2, 55, 0);
      break;

    case Creature::EMO_SADNESS:
    case Creature::EMO_ANGER:al_draw_bitmap_region(status_sprite, 16, 64, 8, 8, 2, 55, 0);
      break;
  }

  // two separate backgrounds that go behind both the thirst and health
  al_draw_bitmap_region(status_sprite, 0, 114, 35, 5, 28, 54, 0);
  al_draw_bitmap_region(status_sprite, 0, 114, 35, 5, 28, 58, 0);

  if (creature->GetThirst() > 0) { // draw thirst meter
    al_draw_bitmap_region(status_sprite, 0, 122, (creature->GetThirst() * 33) / 100, 3, 29, 55, 0);
  }
  if (creature->GetHealth() > 0) { // draw health meter
    al_draw_bitmap_region(status_sprite, 0, 119, (creature->GetHealth() * 33) / 100, 3, 29, 59, 0);
  }

  al_draw_bitmap_region(
      status_sprite, 0, 105, 1, (float) (creature->emotions_[Creature::EMO_HAPPINESS] * 9) / 100, 11, 54, 0);
  al_draw_bitmap_region(
      status_sprite, 1, 105, 1, (float) (creature->emotions_[Creature::EMO_BOREDOM] * 9) / 100, 12, 54, 0);
  al_draw_bitmap_region(
      status_sprite, 2, 105, 1, (float) (creature->emotions_[Creature::EMO_ANGER] * 9) / 100, 13, 54, 0);
  al_draw_bitmap_region(
      status_sprite, 3, 105, 1, (float) (creature->emotions_[Creature::EMO_SADNESS] * 9) / 100, 14, 54, 0);
}

void GameDisplayFrame() {
  al_clear_to_color(al_map_rgb(0, 0, 0));

  World::GetInstance()->Draw();

  creature->Draw();
  toy->Draw();
  drink->Draw();

  DrawMenu();
}

#define CAMERA_MAXSPEED     10
#define CAMERA_ACCELERATION 0.05f

void Game_Tick() {
  if(game.state == GAME_STATE_PAUSED) {
    return;
  }

  World::GetInstance()->Simulate();

  AgentFactory::Get()->Tick();

  creature->Simulate();
  toy->Simulate();
  drink->Simulate();
}

void MouseEvent() {

}

void KeyboardEvent(int code, bool keyup) {
  if (keyup) {
    return;
  }

  switch (code) {
    default: break;

    case ALLEGRO_KEY_PAUSE: {
#if 0
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
#endif
      break;
    }

    case ALLEGRO_KEY_ESCAPE:
#if 0
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
#endif
      exit(0);
  }
}

void ShutdownGame() {}

