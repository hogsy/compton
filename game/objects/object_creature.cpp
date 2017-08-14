
#include "object_creature.h"
#include "../../shared.h"
#include "../game.h"
#include "object_world.h"

#define CREATURE_BOUND          11

#define GROUND_LEVEL   44

#define BORED_SLEEP   50

class SnoozeCloud : public Sprite {
public:
    SnoozeCloud() : Sprite(engine::LoadImage("sprites")) {
        origin_.Set(2.5, 2.5);
    }

    void Draw() override {
        al_draw_bitmap_region(
                GetBitmap(), 0, 72, 5, 5,
                (position_.x - origin_.x), (position_.y - origin_.y),
                0
        );
    }

    void Simulate() {
        position_.y -= 0.07f;
        position_.x += std::sin((engine_vars.counter / 2) * 100) / 2;
    }
};

void CreatureObject::Draw() {
    al_draw_bitmap_region(
            GetBitmap(), s_x_, s_y_, s_w_, s_h_,
            (position_.x - origin_.x), (position_.y - origin_.y),
            0
    );
}

void CreatureObject::Simulate() {
    static bool is_onground = false;

    if(parent_ != nullptr && is_grabbed) {
        return;
    }

    if(!game.is_grabbing) {
        if (!is_grabbed && engine_vars.mouse_status[BUTTON_LMOUSE]) {
            if ((engine_vars.mouse_state.x / 8 > ((position_.x - origin_.x) - 10)) &&
                (engine_vars.mouse_state.x / 8 < ((position_.x - origin_.x) + 10)) &&
                (engine_vars.mouse_state.y / 8 > ((position_.y - origin_.y) - 10)) &&
                (engine_vars.mouse_state.y / 8 < ((position_.y - origin_.y) + 10))) {
                is_grabbed = true;
                game.is_grabbing = true;
            }
        }
    }

    if (is_grabbed) {
        static PLVector2D old_position = { 0, 0 };
        if (!engine_vars.mouse_status[BUTTON_LMOUSE]) {
            is_grabbed = false;
            game.is_grabbing = false;
            velocity_ = (old_position - position_) * -1;
            return;
        }

        old_position = position_;
        position_.x = engine_vars.mouse_state.x / 8;
        position_.y = engine_vars.mouse_state.y / 8;
        return;
    }

    // gravity
    velocity_.y += 0.085f;

    if(velocity_.x > 0) {
        velocity_.x -= 0.05f;
    } else if(velocity_.x < 0) {
        velocity_.x += 0.05f;
    }

    if(is_onground) {
        if (velocity_.x < 0.05f && velocity_.x > -0.05f) {
            velocity_.x = 0;
        }
    }

    if((position_.x <= 0) || (position_.x >= 64)) {
        velocity_.x *= -1;
    }

    position_ += velocity_;

    if(position_.y < 0) {
        position_.y = 0;
        velocity_.y *= -1;
    }

    if(position_.y >= (GROUND_LEVEL + s_h_ - 2)) {
        //  velocity_.y *= -1;
        position_.y = (GROUND_LEVEL + s_h_ - 2);
        is_onground = true;
    } else {
        is_onground = false;
    }

    if(position_.x < 0) {
        position_.x = 0;
        //  velocity_.x *= -1;
    } else if(position_.x > DISPLAY_WIDTH - 1) {
        position_.x = DISPLAY_WIDTH - 1;
        //  velocity_.x *= -1;
    }
}

// toy

void CreatureToy::Simulate() {
    CreatureObject::Simulate();


}

CreatureDrink   *drink = nullptr;
CreatureToy     *toy = nullptr;

///////////////////////////////////////////////////////////////////

Creature::Creature() : Sprite(engine::LoadImage("sprites")) {
    state_ = old_state_ = EMO_INDIFFERENT;

    origin_.Set(CREATURE_BOUND / 2, CREATURE_BOUND / 2);

    velocity_.Clear();
    position_.Set(32, 32);

    is_grabbed = false;

    delay_look_ = rand()%100 + 200;
    delay_movement  = rand()%500 + 500;
    delay_lastmove  = 100;

    delay_sleep_    =
    delay_play_     =
    delay_drink_    = 10;

    directive_ = DIR_NONE;

    ////////////////////////////////////////////

    thirst_ =
    health_ = 100;

#if 0
    memset(&(emotions_), 0, sizeof(float));
#else // kept getting odd behaviour from the above, tried a bunch of things
    for (double &emotion : emotions_) { // ensure threshold is maintained...
        emotion = 0;
    }
#endif

    //emotions_[EMO_BOREDOM] = BORED_SLEEP;
}

#define relative_mousepos(p) (engine_vars.mouse_state.p / 8)

void Creature::Draw() {

    unsigned int s_x, s_y;
    switch (state_) {
        default:
        case EMO_INDIFFERENT:
            s_y = 33;
            break;
        case EMO_ANGER:
            s_y = 21;
            break;
        case EMO_HAPPINESS:
            s_y = 0;
            break;
    }

    static SnoozeCloud *cloud = nullptr;
    if(directive_ == DIR_SLEEP) {
        if (cloud == nullptr) {
            cloud = new SnoozeCloud();
            cloud->position_ = position_;
            cloud->position_.y -= 9;
        }
        s_y = 44;
    }

    if(velocity_.x > 0.9f) {
        s_x = 75;
    } else if(velocity_.x < -0.9f) {
        s_x = 86;
    } else {
        s_x = 64;

#define y_left      11
#define y_right     22
#define y_upleft    33
#define y_upright   44
#define y_up        0

        switch(target_look_) {
            default: {

            }

            case LOO_CURSOR: {
                if ((delay_look_ <= 0) && (directive_ == DIR_NONE)) {
                    if (delay_look_ < -400) {
                        delay_look_ = 200 + (rand() % 200);
                    }

                    if ((relative_mousepos(x) < (position_.x - 20) &&
                            (relative_mousepos(y) < (position_.y - 20)))) {
                        s_x = 108; s_y = y_upleft;
                    } else if ((relative_mousepos(x) > (position_.x + 20) &&
                                (relative_mousepos(y) < (position_.y - 20)))) {
                        s_x = 108; s_y = y_upright;
                    } else if (relative_mousepos(x) < (position_.x - 20)) {
                        s_x = 108; s_y = y_left;
                    } else if (relative_mousepos(x) > (position_.x + 20)) {
                        s_x = 108; s_y = y_right;
                    } else if (relative_mousepos(y) < (position_.y - 20)) {
                        s_x = 108; s_y = y_up;
                    }
                }
            }

            case LOO_OBJECT: {
                if((look_object_ == nullptr) || (grabbed_object_ != nullptr)) {
                    break;
                }

                if ((look_object_->position_.x < (position_.x - 8) &&
                     (look_object_->position_.y < (position_.y - 8)))) {
                    s_x = 108; s_y = y_upleft;
                } else if ((look_object_->position_.x > (position_.x + 8) &&
                            (look_object_->position_.y < (position_.y - 8)))) {
                    s_x = 108; s_y = y_upright;
                } else if (look_object_->position_.x < (position_.x - 8)) {
                    s_x = 108; s_y = y_left;
                } else if (look_object_->position_.x > (position_.x + 8)) {
                    s_x = 108; s_y = y_right;
                } else if (look_object_->position_.y < (position_.y - 8)) {
                    s_x = 108; s_y = y_up;
                }
            }
        }
    }

    if(cloud != nullptr) {
        cloud->Draw();
        cloud->Simulate();
        if(cloud->position_.y < -10) {
            delete cloud;
            cloud = nullptr;
        }
    }

    al_draw_bitmap_region(
            GetBitmap(), s_x, s_y, CREATURE_BOUND, CREATURE_BOUND,
            (position_.x - origin_.x), (position_.y - origin_.y),
            0
    );
}

/*
 * engine_vars.mouse_state.x, engine_vars.mouse_state.y
 */

void Creature::Simulate() {

    // PHYSICS ......................................................................

    static bool is_onground = false;

    if(grabbed_object_ != nullptr) {
        grabbed_object_->position_ = position_;
        grabbed_object_->position_.y += 1;
#if 0
        if(velocity_.x > 0.01f) {
            grabbed_object_->position_.x += (7 - grabbed_object_->origin_.x);
        } else if(velocity_.x < -0.01f) {
            grabbed_object_->position_.x -= (7 - grabbed_object_->origin_.x);
        }
#endif
    }

    if(!game.is_grabbing) {
        if (!is_grabbed && engine_vars.mouse_status[BUTTON_LMOUSE]) {
            if ((relative_mousepos(x) > ((position_.x - origin_.x) - 10)) &&
                (relative_mousepos(x) < ((position_.x - origin_.x) + 10)) &&
                (relative_mousepos(y) > ((position_.y - origin_.y) - 10)) &&
                (relative_mousepos(y) < ((position_.y - origin_.y) + 10))) {
                is_grabbed = true;
                game.is_grabbing = true;
            }
        }
    }

    if (is_grabbed) {
        static PLVector2D old_position = { 0, 0 };
        if (!engine_vars.mouse_status[BUTTON_LMOUSE]) {
            is_grabbed = false;
            game.is_grabbing = false;
            velocity_ = (old_position - position_) * -1;
            return;
        }

        old_position = position_;
        position_.x = relative_mousepos(x);
        position_.y = relative_mousepos(y);
        return;
    }

    // gravity
    velocity_.y += 0.085f;

    if(velocity_.x > 0) {
        velocity_.x -= 0.05f;
    } else if(velocity_.x < 0) {
        velocity_.x += 0.05f;
    }

    if(is_onground) {
        if (velocity_.x < 0.05f && velocity_.x > -0.05f) {
            velocity_.x = 0;
        }
    }

    if((position_.x <= 0) || (position_.x >= 64)) {
        velocity_.x *= -1;
    }

    if(directive_ == DIR_NONE) {
        if (delay_movement <= 0 && delay_lastmove <= 0) {
            if(is_onground) {
                velocity_.y = -0.8f;
            }

            if (position_.x > 54) {
                velocity_.x -= 2;
            } else if(position_.x < 10) {
                velocity_.x += 2;
            } else {
                if(position_.x > 32) {
                    velocity_.x -= 0.9f + (float)((rand() % 50) / 50);
                } else {
                    velocity_.x += 0.9f + (float)((rand() % 50) / 50);
                }
            }

            delay_lastmove = 100;
        }
        delay_lastmove--;
    }

    position_ += velocity_;

    if(position_.y < 0) {
        position_.y = 0;
        velocity_.y *= -1;
    }

    if(position_.y >= GROUND_LEVEL) {
      //  velocity_.y *= -1;
        position_.y = GROUND_LEVEL;
        is_onground = true;
    } else {
        is_onground = false;
    }

    if(position_.x < 0) {
        position_.x = 0;
      //  velocity_.x *= -1;
    } else if(position_.x > DISPLAY_WIDTH - 1) {
        position_.x = DISPLAY_WIDTH - 1;
      //  velocity_.x *= -1;
    }

    // LOGIC ......................................................................

    if(health_ == 0) { // dead logic
        if(grabbed_object_ != nullptr) {
            DropObject();
        }
        return;
    }

    for (double &emotion : emotions_) { // ensure threshold is maintained...
        if(emotion < 0) {
            emotion = 0;
        } else if(emotion > 100) {
            emotion = 100;
        }
    }

    if(emotions_[EMO_HAPPINESS] > 50) {
        SetState(EMO_HAPPINESS);
    } else if(emotions_[EMO_SADNESS] > 50) {
        SetState(EMO_SADNESS);
    } else if(emotions_[EMO_ANGER] > 50) {
        SetState(EMO_ANGER);
    } else {
        SetState(EMO_INDIFFERENT);
    }

    switch(directive_) {
        default: {
#if 1
            if(delay_look_ < World::GetInstance()->GetTotalSeconds()) {
                if (toy->is_grabbed && (toy->parent_ == nullptr)) {
                    target_look_ = LOO_OBJECT;
                    look_object_ = toy;
                } else if(drink->is_grabbed && (drink->parent_ == nullptr)) {
                    target_look_ = LOO_OBJECT;
                    look_object_ = drink;
                } else {
                    target_look_ = LOO_CURSOR;
                    look_object_ = nullptr;
                }

                delay_look_ = World::GetInstance()->GetTotalSeconds() + 50;
            }

            delay_movement--;
            if (delay_movement < -500) {
                delay_movement = rand() % 500 + 500;
            }
#endif

            if((thirst_ < 50) && (delay_drink_ < World::GetInstance()->GetTotalSeconds())) {
                directive_ = DIR_DRINK;
            } else if((emotions_[EMO_BOREDOM] > 20) && (delay_sleep_ < World::GetInstance()->GetTotalSeconds())) {
                directive_ = DIR_SLEEP;
            } else if((emotions_[EMO_HAPPINESS] < 50) && (delay_play_ < World::GetInstance()->GetTotalSeconds())) {
                directive_ = DIR_PLAY;
                delay_play_ = World::GetInstance()->GetTotalSeconds() + 2000;
            }

            emotions_[EMO_BOREDOM]      += 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        case DIR_DRINK: {
            look_object_ = drink;
            target_look_ = LOO_OBJECT;

            // hop towards object
            auto *cur_drink = dynamic_cast<CreatureDrink*>(grabbed_object_);
            if(cur_drink == nullptr) {
                if(grabbed_object_ != nullptr) {
                    DropObject();
                }

                if (!(
                        ((drink->position_.x - drink->origin_.x) > ((position_.x - origin_.x) - 9)) &&
                        ((drink->position_.x - drink->origin_.x) < ((position_.x - origin_.x) + 9)) &&
                        ((drink->position_.y - drink->origin_.y) > ((position_.y - origin_.y) - 9)) &&
                        ((drink->position_.y - drink->origin_.y) < ((position_.y - origin_.y) + 9)))) {
                    if (is_onground) {
                        velocity_.y = -0.9f;
                        if (position_.x < drink->position_.x) {
                            velocity_.x += 0.8f;
                        } else {
                            velocity_.x -= 0.8f;
                        }
                    }
                    break;
                }

                // pick it up
                PickupObject(drink);
            }

            if(thirst_ >= 100) {
                directive_ = DIR_NONE;

                delay_drink_    = World::GetInstance()->GetTotalSeconds() + 50;
                //delay_play_     = World::GetInstance()->GetTotalSeconds() + 4000;
                delay_sleep_    = World::GetInstance()->GetTotalSeconds() + 100;

                DropObject();
                ClearLook();

                if (is_onground) {
                    velocity_.y = -0.9f;
                    if(position_.x < 32) {
                        velocity_.x += 1.f;
                    } else {
                        velocity_.x -= 1.f;
                    }
                }
            }
            thirst_ += 0.5f;

            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        case DIR_PLAY: {
            look_object_ = toy;
            target_look_ = LOO_OBJECT;

            if((thirst_ < 30) || (emotions_[EMO_HAPPINESS] > 80)) {
                DropObject();
                ClearLook();

                delay_sleep_    = World::GetInstance()->GetTotalSeconds() + 1000;
                delay_drink_    = World::GetInstance()->GetTotalSeconds() + 100;
                delay_play_     = World::GetInstance()->GetTotalSeconds() + 5000;

                directive_ = DIR_NONE;
                break;
            }

            if(delay_throw_ < 0) {
                delay_throw_ -= 0.1f;
                if(delay_throw_ < -5.f) {
                    delay_throw_ = World::GetInstance()->GetTotalSeconds() + (10 + (rand() % 10));
                }
                break;
            }

            // hop towards object
            auto *cur_toy = dynamic_cast<CreatureToy*>(grabbed_object_);
            if(cur_toy == nullptr) {
                if(grabbed_object_ != nullptr) {
                    DropObject();
                }

                if (!(
                        ((toy->position_.x - toy->origin_.x) > ((position_.x - origin_.x) - 9)) &&
                        ((toy->position_.x - toy->origin_.x) < ((position_.x - origin_.x) + 9)) &&
                        ((toy->position_.y - toy->origin_.y) > ((position_.y - origin_.y) - 9)) &&
                        ((toy->position_.y - toy->origin_.y) < ((position_.y - origin_.y) + 9)))) {
                    if (is_onground) {
                        velocity_.y = -0.9f;
                        if (position_.x < toy->position_.x) {
                            velocity_.x += 0.8f;
                        } else {
                            velocity_.x -= 0.8f;
                        }
                    }
                    break;
                }

                PickupObject(toy);

                delay_throw_ = World::GetInstance()->GetTotalSeconds() + (10 + (rand() % 10));
            }

            if (is_onground) {
                velocity_.y = -1.05f;
                if(relative_mousepos(x) < position_.x) {
                    if(position_.x > 54) {
                        velocity_.x -= 1.5f;
                    } else {
                        velocity_.x += 0.7f;
                    }
                } else {
                    if(position_.x < 10) {
                        velocity_.x += 1.5f;
                    } else {
                        velocity_.x -= 0.7f;
                    }
                }

                if(delay_throw_ < World::GetInstance()->GetTotalSeconds()) {
#if 0
                    if(relative_mousepos(x) < 0 || relative_mousepos(x) > 64) {
#endif
                        grabbed_object_->velocity_.y = -2.f;
                        if(position_.x > 32) {
                            grabbed_object_->velocity_.x -= 1.5f;
                        } else {
                            grabbed_object_->velocity_.x += 1.5f;
                        }
#if 0
                    } else {
                        PLVector2D mpos(relative_mousepos(x), relative_mousepos(y));
                        grabbed_object_->velocity_ = ((position_ - mpos) * -1) / 100;
                    }
#endif

                    DropObject();

                    delay_throw_ = -1;
                }
            }

            // todo, pick object up...
            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    += 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        case DIR_SLEEP: {
            static unsigned int sleep_start = 0, sleep_end = 0;
            if(sleep_start == 0) {
                sleep_start = World::GetInstance()->GetTotalSeconds();
                sleep_end = sleep_start + 1500;
            }

            if(sleep_end < World::GetInstance()->GetTotalSeconds()) {
                sleep_start = 0;

                delay_sleep_    = World::GetInstance()->GetTotalSeconds() + 5000;
                delay_drink_    = World::GetInstance()->GetTotalSeconds() + 2000;
                delay_play_     = World::GetInstance()->GetTotalSeconds() + 4000;

                directive_ = DIR_NONE;
            }

            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }
    }

    if(thirst_ < 1) {
        health_ -= 0.05f;
    }

    if(health_ < 0) {
        health_ = 0;
    } else if(health_ > 100) {
        health_ = 100;
    }

    auto drinking = dynamic_cast<CreatureDrink*>(grabbed_object_);
    if(drinking == nullptr) {
        thirst_ -= 0.05f;
    }

    if(thirst_ < 0) {
        thirst_ = 0;
    } else if(thirst_ > 100) {
        thirst_ = 100;
    }
}

void Creature::DropObject() {
    if(grabbed_object_ == nullptr) {
        return;
    }

    grabbed_object_->parent_ = nullptr;
    grabbed_object_->is_grabbed = false;
    grabbed_object_ = nullptr;
}

void Creature::PickupObject(CreatureObject *object) {
    grabbed_object_ = object;
    grabbed_object_->parent_ = this;
    grabbed_object_->is_grabbed = true;
}

Creature *creature = nullptr;
