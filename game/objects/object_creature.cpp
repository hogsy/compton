
#include "object_creature.h"
#include "../../shared.h"
#include "../game.h"

#define CREATURE_BOUND          11
#define CREATURE_LOWEST_POINT   39

Creature::Creature() : Sprite(engine::LoadImage("sprites")) {
    current_state_ = old_state_ = CREATURE_STATE_INDIFFERENT;
    current_mode_ = old_mode_ = HAND_MODE_DEFAULT;

    origin_.Set(CREATURE_BOUND / 2, CREATURE_BOUND / 2);

    velocity_.Clear();
    position_.Set(32, -10);

    memset(emotions_, 0, sizeof(float) * CREATURE_STATE_END);
}

void Creature::Draw() {
    switch (current_state_) {
        default:
        case CREATURE_STATE_INDIFFERENT:
            al_draw_bitmap_region(GetBitmap(), 64, 36, CREATURE_BOUND, CREATURE_BOUND,
                                  (position_.x - origin_.x), position_.y,
                                  0);
            break;
        case CREATURE_STATE_ANGRY:
            al_draw_bitmap_region(GetBitmap(), 64, 24, CREATURE_BOUND, CREATURE_BOUND,
                                  position_.x, position_.y,
                                  0);
            break;
        case CREATURE_STATE_HAPPY:
            al_draw_bitmap_region(GetBitmap(), 64, 0, CREATURE_BOUND, CREATURE_BOUND,
                                  position_.x, position_.y,
                                  0);
            break;
        case CREATURE_STATE_SAD:
            al_draw_bitmap_region(GetBitmap(), 64, 48, CREATURE_BOUND, CREATURE_BOUND,
                                  position_.x, position_.y,
                                  0);
            break;
        case CREATURE_STATE_SURPRISED:
            al_draw_bitmap_region(GetBitmap(), 64, 12, CREATURE_BOUND, CREATURE_BOUND,
                                  position_.x, position_.y,
                                  0);
            break;
    }
}

/*
 * engine_vars.mouse_state.x, engine_vars.mouse_state.y
 */

void Creature::Simulate() {
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

    position_ += velocity_;
    if(position_.y > CREATURE_LOWEST_POINT) {
        position_.y = CREATURE_LOWEST_POINT;
    }

    switch (current_state_) {

        default:
            break;
    }

    if (angle != target_angle_) {
        angle = plCosineInterpolate(angle, target_angle_, move_time_ / 6);
    }
}

