
#pragma once
#include "../../shared.h"
#include "../../engine/sprite.h"

class Creature : public Sprite {
public:
    enum {
        CREATURE_STATE_HAPPY,
        CREATURE_STATE_SURPRISED,
        CREATURE_STATE_ANGRY,
        CREATURE_STATE_INDIFFERENT,
        CREATURE_STATE_SAD,

        CREATURE_STATE_END
    };

    enum {
        HAND_MODE_DEFAULT,
        HAND_MODE_DRAG,
    };

    Creature();

    void SetState(unsigned int state) {
        old_state_ = current_state_;
        current_state_ = state;
    }

    void SetMode(unsigned int mode) {
        old_mode_ = current_mode_;
        current_mode_ = mode;
    }

    void Draw() override;
    void Simulate();

protected:
private:
    float move_time_{}, transition_delay_, target_angle_{};
    float emotions_[CREATURE_STATE_END];

    unsigned int current_state_, old_state_;

    PLVector2D velocity_;
    PLVector2D old_position_;

    unsigned int current_mode_, old_mode_;
};