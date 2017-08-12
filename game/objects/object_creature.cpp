
#include "object_creature.h"
#include "../../shared.h"
#include "../game.h"

#define CREATURE_BOUND          11
#define CREATURE_LOWEST_POINT   45

Creature::Creature() : Sprite(engine::LoadImage("sprites")) {
    current_state_ = old_state_ = CREATURE_STATE_INDIFFERENT;
    current_mode_ = old_mode_ = HAND_MODE_DEFAULT;

    origin_.Set(CREATURE_BOUND / 2, CREATURE_BOUND / 2);

    velocity_.Clear();
    position_.Set(32, -10);

    delay_mouselook = 300;

    memset(emotions_, 0, sizeof(float) * CREATURE_STATE_END);
}

void Creature::Draw() {
    if(delay_mouselook <= 0) {
        if(delay_mouselook < -400) {
            delay_mouselook = 200 + (rand() % 200);
        }

        if(engine_vars.mouse_state.x / 8 < (position_.x - 20)) {
            al_draw_bitmap_region(GetBitmap(), 112, 24, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            emotions_[CREATURE_STATE_HAPPY] += 0.5f;
            return;
        } else if(engine_vars.mouse_state.x / 8 > (position_.x + 20)) {
            al_draw_bitmap_region(GetBitmap(), 112, 36, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            emotions_[CREATURE_STATE_HAPPY] += 0.5f;
            return;
        } else if(engine_vars.mouse_state.y / 8 < (position_.y - 20)) {
            al_draw_bitmap_region(GetBitmap(), 112, 12, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            emotions_[CREATURE_STATE_HAPPY] += 0.5f;
            return;
        }
    }

    switch (current_state_) {
        default:
        case CREATURE_STATE_INDIFFERENT:
            al_draw_bitmap_region(GetBitmap(), 64, 36, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            break;
        case CREATURE_STATE_ANGRY:
            al_draw_bitmap_region(GetBitmap(), 64, 24, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            break;
        case CREATURE_STATE_HAPPY:
            al_draw_bitmap_region(GetBitmap(), 64, 0, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            break;
        case CREATURE_STATE_SAD:
            al_draw_bitmap_region(GetBitmap(), 64, 48, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            break;
        case CREATURE_STATE_SURPRISED:
            al_draw_bitmap_region(GetBitmap(), 64, 12, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), (position_.y - origin_.y),
                                  0);
            break;
    }
}

/*
 * engine_vars.mouse_state.x, engine_vars.mouse_state.y
 */

void Creature::Simulate() {

    delay_mouselook--;

    // gravity
    velocity_.y += 0.09f;

    if(velocity_.x > 0) {
        velocity_.x -= 0.05f;
    } else if(velocity_.x < 0) {
        velocity_.x += 0.05f;
    }

    if(position_.x <= 0) {
        velocity_.x = velocity_.x * -1;
    } else if(position_.x >= 64) {
        velocity_.x = -velocity_.x;
    }

    static PLVector2D oldpos = { 0, 0 };
    if(engine_vars.mouse_status[BUTTON_LMOUSE]) {
        if(!is_grabbed) {
            if( (engine_vars.mouse_state.x / 8 > (position_.x - 3)) &&
                (engine_vars.mouse_state.x / 8 < (position_.x + 3)) &&
                (engine_vars.mouse_state.y / 8 > (position_.y - 3)) &&
                (engine_vars.mouse_state.y / 8 < (position_.y + 3))) {
                is_grabbed = true;
            }
        } else {
            oldpos = position_;
            position_.x = engine_vars.mouse_state.x / 8;
            position_.y = engine_vars.mouse_state.y / 8;
            velocity_.Clear();
        }
    } else if(is_grabbed) {
        is_grabbed = false;

        PLVector2D vel_dir = (oldpos - position_);
        velocity_ = vel_dir * -1;

        return;
    } else { // only do this if we're not grabbed
        position_ += velocity_;
    }

    if(position_.y < 0) {
        position_.y = 0;
        velocity_.y *= -1;
    }
    if(position_.y >= CREATURE_LOWEST_POINT) {
        velocity_.y -= 1.15f;
        position_.y = CREATURE_LOWEST_POINT;
    }

    if(position_.x < 0) {
        position_.x = 0;
        velocity_.x *= -1;
    } else if(position_.x > DISPLAY_WIDTH - 1) {
        position_.x = DISPLAY_WIDTH - 1;
        velocity_.x *= -1;
    }

    switch (current_state_) {

        default:
            break;
    }

    if (angle != target_angle_) {
        angle = plCosineInterpolate(angle, target_angle_, move_time_ / 6);
    }

    // emotion handlers...
    if(emotions_[CREATURE_STATE_HAPPY] < 50) {
        emotions_[CREATURE_STATE_INDIFFERENT] += 0.5f;
    }

    if(emotions_[CREATURE_STATE_HAPPY] >= 100) {
        SetState(CREATURE_STATE_HAPPY);
    } else if(emotions_[CREATURE_STATE_INDIFFERENT] >= 100) {
        SetState(CREATURE_STATE_INDIFFERENT);
    }
}

