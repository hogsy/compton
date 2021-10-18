/*
SimGame Engine
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

#include "../shared.h"

#include "game.h"

/*	Game logic and other crap goes here!	*/

GameVars game;

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

void DrawStatusBar( ALLEGRO_COLOR colour, unsigned int value, float x, float y )
{
	//al_draw_bitmap_region( status_sprite, 0, 114, 35, 5, x, y, 0 );
	//if ( creature->GetHealth() > 0 ) {// draw health meter
	//	al_draw_tinted_bitmap_region( status_sprite, colour, 0, 119, ( float ) ( value ) *33 / 100, 3, x + 1, y + 1, 0 );
	//}
}

void DrawMenu()
{
#if 0
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
           World::Get()->GetYear(),
           World::Get()->GetMonth(),
           World::Get()->GetDay()
  );
  DrawShadowString(game.font_small, static_cast<int>(x_scroll), 10, al_map_rgb(255, 255, 255), monthstr);
#else
  std::string cur_day = "DAY " + std::to_string(World::Get()->GetTotalDays() + 1);
  DrawShadowString(game.font_small, 2, 10, al_map_rgb(255, 255, 255), cur_day.c_str());
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
           World::Get()->GetHour(), blink,
           World::Get()->GetMinute()); // Removed the second timer here...
  DrawShadowString(game.font_small, 2, 2, al_map_rgb(255, 255, 255), timestr);
  blink_delay++;

  float emo_y = 227;
  switch (creature->GetState()) {
    default:
    case Creature::EMO_HAPPINESS:al_draw_bitmap_region(status_sprite, 0, 64, 8, 8, 10, emo_y, 0);
      break;

    case Creature::EMO_INDIFFERENT:al_draw_bitmap_region(status_sprite, 8, 64, 8, 8, 10, emo_y, 0);
      break;

    case Creature::EMO_SADNESS:
    case Creature::EMO_ANGER:al_draw_bitmap_region(status_sprite, 16, 64, 8, 8, 10, emo_y, 0);
      break;
  }

  // health meter
  const float bar_x = 28;
  float bar_y = 226;
  DrawStatusBar(al_map_rgb(255, 0, 0), creature->GetHealth(), bar_x, bar_y);
  bar_y += 6;
  DrawStatusBar(al_map_rgb(0, 0, 255), creature->GetThirst(), bar_x, bar_y);

  emo_y = 54;
  al_draw_bitmap_region(
      status_sprite, 0, 105, 1, (float) (creature->emotions_[Creature::EMO_HAPPINESS] * 9) / 100, 11, emo_y, 0);
  al_draw_bitmap_region(
      status_sprite, 1, 105, 1, (float) (creature->emotions_[Creature::EMO_BOREDOM] * 9) / 100, 12, emo_y, 0);
  al_draw_bitmap_region(
      status_sprite, 2, 105, 1, (float) (creature->emotions_[Creature::EMO_ANGER] * 9) / 100, 13, emo_y, 0);
  al_draw_bitmap_region(
      status_sprite, 3, 105, 1, (float) (creature->emotions_[Creature::EMO_SADNESS] * 9) / 100, 14, emo_y, 0);
#endif
}
