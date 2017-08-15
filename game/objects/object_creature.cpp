
#include "object_creature.h"
#include "../../shared.h"
#include "../game.h"
#include "object_world.h"

#define CREATURE_BOUND          11

#define GROUND_LEVEL   44

#define BORED_SLEEP   50

#define get_gametime() (World::GetInstance()->GetTotalSeconds())

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
            s_y = 22;
            break;
        case EMO_HAPPINESS:
            s_y = 0;
            break;
    }

    static SnoozeCloud *cloud = nullptr;
    if(occupation_ == OCU_SLEEPING) {
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
                break;
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
                break;
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

    if(directive_ == DIR_SHOCKED) {
        s_x = 75; s_y = 55;
    } else if(directive_ == DIR_DEAD) {
        s_x = 64; s_y = 55;
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

    if(is_grounded_) {
        if (velocity_.x < 0.05f && velocity_.x > -0.05f) {
            velocity_.x = 0;
        }
    }

    if((position_.x <= 0) || (position_.x >= 64)) {
        velocity_.x *= -1;

        Impact();
    }

    position_ += velocity_;

    if(position_.y < 0) {
        position_.y = 0;
        velocity_.y *= -1;
    }

    if(position_.y >= GROUND_LEVEL) {
      //  velocity_.y *= -1;
        position_.y = GROUND_LEVEL;
        if(!is_grounded_) {
            Impact();
            is_grounded_ = true;
        }
    } else {
        is_grounded_ = false;
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

#if 0

    switch(directive_) {
        default: {
#if 1


            delay_movement--;
            if (delay_movement < -500) {
                delay_movement = rand() % 500 + 500;
            }
#endif

            if((thirst_ < 50) && (delay_drink_ < World::GetInstance()->GetTotalSeconds())) {
                SetDirective(DIR_DRINK);
                break;
            } else if(((emotions_[EMO_BOREDOM] > 20) && (delay_sleep_ < World::GetInstance()->GetTotalSeconds()) ||
                        delay_sleep_ < World::GetInstance()->GetTotalSeconds() - 50)) {
                SetDirective(DIR_SLEEP);
                break;
            } else if((emotions_[EMO_HAPPINESS] < 50) && (delay_play_ < World::GetInstance()->GetTotalSeconds())) {
                SetDirective(DIR_PLAY);
                break;
            }

            emotions_[EMO_BOREDOM]      += 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        case DIR_RAGE: {
            ClearLook();
            target_look_ = LOO_CURSOR;

            DropObject();

            Jump(0.8f);
            if((thirst_ < 50) && (delay_drink_ < World::GetInstance()->GetTotalSeconds())) {
                directive_ = DIR_DRINK;
                break;
            } else if(emotions_[EMO_ANGER] <= 0) {
                directive_ = DIR_NONE;
                break;
            }

            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        case DIR_SHOCKED: {
            ClearLook();

            DropObject();

            if((delay_play_ < World::GetInstance()->GetTotalSeconds())) {
                break;
            }

            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.5f;
            emotions_[EMO_SADNESS]      += 0.5f;
            break;
        }

        case DIR_SLEEP: {

        }
    }

#else

    if(occupation_ == OCU_NONE) {
        if ((thirst_ < 30) && (delay_drink_ < get_gametime())) {
            directive_ = DIR_DRINK;
        }

        switch(state_) {
            default: {
                if(emotions_[EMO_ANGER] > 50) {
                    directive_ = DIR_RAGE;
                    break;
                }

                if((emotions_[EMO_BOREDOM] > 65) && (delay_play_ < get_gametime())) {
                    directive_ = DIR_PLAY;
                    break;
                }
            } break;

            case EMO_HAPPINESS: {
                if((emotions_[EMO_HAPPINESS] > 50) && (delay_sleep_ < get_gametime())) {
                    directive_ = DIR_SLEEP;
                    break;
                }
            } break;
        }
    }

    double prev_emo = 0; unsigned int h_emo = 0;
    for(unsigned int i = 0; i < EMO_END; ++i) {
        if(emotions_[i] > prev_emo) {
            h_emo = i;
            prev_emo = emotions_[i];
        }
    }

    if(h_emo == EMO_END) {
        h_emo = EMO_INDIFFERENT;
    }

    SetState(h_emo);

    switch (directive_) {

        case DIR_PLAY: {
            look_object_    = toy;
            target_look_    = LOO_OBJECT;
            occupation_     = OCU_PLAYING;

            if((thirst_ < 30) || (emotions_[EMO_HAPPINESS] >= 100)) {
                ClearLook();

                directive_  = DIR_NONE;
                occupation_ = OCU_NONE;

                delay_play_ = get_gametime() + 100;
                break;
            }

            if(grabbed_object_ == toy) {
                if (is_grounded_) {
                    if (delay_throw_ < World::GetInstance()->GetTotalSeconds()) {
                        ThrowObject();
                        emotions_[EMO_HAPPINESS] += 10.f;

                        delay_throw_    = get_gametime() + 100;
                        delay_movement  = get_gametime() + 50;
                    }
                }
                emotions_[EMO_ANGER] -= 0.05f;
            } else if(toy->is_grabbed && (toy->parent_ != this)) {
                if(delay_throw_ < get_gametime()) {
                    emotions_[EMO_ANGER] += 5.f;
                }
            }

            emotions_[EMO_BOREDOM]  -= 0.05f;
            emotions_[EMO_SADNESS]  -= 0.05f;
            break;
        }

        case DIR_SLEEP: {
            if(occupation_ != OCU_SLEEPING) {
                ClearLook();

                occupation_ = OCU_SLEEPING;
#
                delay_sleep_ = get_gametime() + 500;
            }

            if(is_grabbed) { // try again later...
                emotions_[EMO_ANGER] += 20.f;

                occupation_ = OCU_NONE;
                directive_  = DIR_NONE;

                delay_sleep_ = get_gametime() + 100;
                break;
            }

            if(delay_sleep_ < get_gametime()) {
                occupation_ = OCU_NONE;
                directive_  = DIR_NONE;

                delay_sleep_ = get_gametime() + 400;
                break;
            }

            if(((thirst_ < 20) && (delay_drink_ < get_gametime()))) {
                occupation_ = OCU_NONE;
                directive_  = DIR_DRINK;

                if((delay_sleep_ - get_gametime()) > 0) {
                    delay_sleep_ = get_gametime() + (delay_sleep_ - get_gametime());
                } else {
                    delay_sleep_ = get_gametime() + 200;
                }
                break;
            }

            delay_movement = get_gametime() + 50;

            emotions_[EMO_BOREDOM]      -= 0.1f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
        } break;

        case DIR_RAGE: {
            ClearLook();

            if(occupation_ != OCU_RAGING) {
                occupation_ = OCU_RAGING;

                delay_look_ = get_gametime() + 400;
            }

            if(delay_look_ < get_gametime()) {
                occupation_ = OCU_NONE;
                directive_  = DIR_NONE;

            }

            Jump(0.7f);

            emotions_[EMO_BOREDOM]      -= 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.1f;
            emotions_[EMO_SADNESS]      -= 0.05f;
        } break;

        case DIR_DRINK: {
            look_object_    = drink;
            target_look_    = LOO_OBJECT;
            occupation_     = OCU_DRINKING;

            if(grabbed_object_ == drink) {
                thirst_ += 0.5f;
                if(thirst_ >= 100) {
                    ClearLook();

                    directive_  = DIR_NONE;
                    occupation_ = OCU_NONE;

                    DropObject();

                    if (is_grounded_) {
                        velocity_.y = -0.9f;
                        if(position_.x < drink->position_.x) {
                            velocity_.x += 1.f;
                        } else {
                            velocity_.x -= 1.f;
                        }
                    }

                    delay_drink_ = World::GetInstance()->GetTotalSeconds() + 100;
                    break;
                }

                // keep still while we drink
                delay_movement = World::GetInstance()->GetTotalSeconds() + 10;
            }

            emotions_[EMO_BOREDOM]      += 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }

        default: {
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

            emotions_[EMO_BOREDOM]      += 0.05f;
            emotions_[EMO_HAPPINESS]    -= 0.05f;
            emotions_[EMO_ANGER]        -= 0.05f;
            emotions_[EMO_SADNESS]      -= 0.05f;
            break;
        }
    }

#define relative_pos_x(p) (p->position_.x - p->origin_.x)
#define relative_pos_y(p) (p->position_.y - p->origin_.y)

    if(delay_movement < World::GetInstance()->GetTotalSeconds()) { // movement
        if(grabbed_object_ == nullptr && look_object_ != nullptr) {
            if(!look_object_->is_grabbed) {
                if (!(relative_pos_x(look_object_) > (relative_pos_x(this) - 9) &&
                      (relative_pos_x(look_object_) < (relative_pos_x(this) + 9)) &&
                      (relative_pos_y(look_object_) > (relative_pos_y(this) - 9)) &&
                      (relative_pos_y(look_object_) < (relative_pos_y(this) + 9)))) {
                    if (is_grounded_) {
                        velocity_.y = -0.9f;
                        if (position_.x < look_object_->position_.x) {
                            velocity_.x += 0.8f;
                        } else {
                            velocity_.x -= 0.8f;
                        }
                    }
                } else if (!PickupObject(look_object_)) { // attempt to pick it up
                    ClearLook();
                    //delay_movement = World::GetInstance()->GetTotalSeconds() + 50;
                } else {
                    delay_throw_ = World::GetInstance()->GetTotalSeconds() + 50;
                }
            }
        } else if (grabbed_object_ != nullptr && grabbed_object_ != look_object_) {
            if ((delay_throw_ < World::GetInstance()->GetTotalSeconds()) && (emotions_[EMO_ANGER] > 50)) {
                ThrowObject();
            } else {
                DropObject();
            }

            delay_movement = World::GetInstance()->GetTotalSeconds() + 50;
        } else {
            static unsigned int move_dir = 0; // 0 - left, 1 - right, 2 - middle

            switch(directive_) {

                default: {
                    if(delay_lastmove < World::GetInstance()->GetTotalSeconds()) {
                        if(is_grounded_) {
                            velocity_.y = -0.8f;
                            if(move_dir == 0) {
                                velocity_.x -= 0.9f;
                                if(position_.x < 20) {
                                    move_dir = 1;
                                }
                            } else if(move_dir == 1) {
                                velocity_.x += 0.9f;
                                if(position_.x > 40) {
                                    move_dir = 0;
                                }
                            } else {
                                if (position_.x > 54) {
                                    velocity_.x -= 2;
                                } else if (position_.x < 10) {
                                    velocity_.x += 2;
                                } else {
                                    if (position_.x > 32) {
                                        velocity_.x -= 0.9f + (float) ((rand() % 50) / 50);
                                    } else {
                                        velocity_.x += 0.9f + (float) ((rand() % 50) / 50);
                                    }
                                }
                            }
                        }
                        delay_lastmove = World::GetInstance()->GetTotalSeconds() + 30;
                    }
                } break;

                case DIR_DRINK:break;
            }

            if(delay_movement < World::GetInstance()->GetTotalSeconds() - 100) {
                move_dir = static_cast<unsigned int>(rand() % 2);

                delay_movement = World::GetInstance()->GetTotalSeconds() + 100;
            }
        }
    }

#endif

    auto drinking = dynamic_cast<CreatureDrink*>(grabbed_object_);
    if(drinking == nullptr) { // drinking from bowel
        thirst_ -= 0.01f;
    } else if(thirst_ < 1) { // dying of thirst
        health_ -= 0.05f;
    }
    if(thirst_ < 0) {
        thirst_ = 0;
    } else if(thirst_ > 100) {
        thirst_ = 100;
    }

    if(health_ < 0) {
        health_ = 0;
    } else if(health_ > 100) {
        health_ = 100;
    }
}

////////////////////////////////////////////////////////

void Creature::DropObject() {
    if(grabbed_object_ == nullptr) {
        return;
    }

    grabbed_object_->parent_ = nullptr;
    grabbed_object_->is_grabbed = false;
    grabbed_object_ = nullptr;

    al_play_sample(game.sample_pickup, 0.5, ALLEGRO_AUDIO_PAN_NONE, 0.2, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void Creature::ThrowObject() {
    if(grabbed_object_ == nullptr) {
        return;
    }

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

    delay_throw_ = World::GetInstance()->GetTotalSeconds() + 10;

    DropObject();

    al_play_sample(game.sample_throw, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

bool Creature::PickupObject(CreatureObject *object) {
    // this should fix our grabby claws mixing with the creature's
    if(object->is_grabbed && (object->parent_ != this)) {
        return false;
    } else if(object->is_grabbed && (object->parent_ == this)) {
        return true;
    }

    grabbed_object_ = object;
    grabbed_object_->parent_ = this;
    grabbed_object_->is_grabbed = true;

    al_play_sample(game.sample_pickup, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    if(grabbed_object_ == drink) {
        al_play_sample(game.sample_charge, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    return true;
}

////////////////////////////////////////////////////////

void Creature::Jump(float velocity) {
    if(!is_grounded_) {
        return;
    }
    // need to inverse velocity, because negative is up (hindsight)
    velocity_.y = -velocity;

    al_play_sample(game.sample_jump, 1, ALLEGRO_AUDIO_PAN_NONE, ((rand() % 5) / 10) + 10, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void Creature::Impact() {
    if((velocity_.y < 0.5f) && (velocity_.y > -0.5f) && (velocity_.x < 0.5f) && (velocity_.x > -0.5f)) {
        return;
    }

    al_play_sample(
            game.sample_land,
            (((velocity_.Length()) / 1000) + 0.5f),
            ALLEGRO_AUDIO_PAN_NONE,
            static_cast<float>(((rand() % 50) / 100) + 0.5),
            ALLEGRO_PLAYMODE_ONCE,
            NULL
    );
}

void Creature::WakeUp() {
    SetDirective(DIR_NONE);
}

void Creature::CheckRage() {
    if(emotions_[EMO_ANGER] != 100 || (emotions_[EMO_BOREDOM] < 50 && emotions_[EMO_ANGER] < 50)) {
        return;
    }

    SetDirective(DIR_RAGE);
}

void Creature::SetDirective(unsigned int dir) {
    switch(directive_) {
        default:break;

        case DIR_SHOCKED: {

        } break;

        case DIR_PLAY: {
            DropObject();
            ClearLook();

            delay_play_ = World::GetInstance()->GetTotalSeconds() + 100;
        } break;

        case DIR_RAGE: {

        } break;

        case DIR_SLEEP: {
            if(occupation_ == OCU_SLEEPING) { // only do this if we're actually sleeping!
                delay_sleep_ = World::GetInstance()->GetTotalSeconds() + 50;
            }
        } break;

        case DIR_DRINK: {
            ClearLook();

            delay_drink_ = World::GetInstance()->GetTotalSeconds() + 35;
        } break;
    }
    directive_ = dir;
}

void Creature::SetOccupation(unsigned int ocu) {
    occupation_ = ocu;
}

Creature *creature = nullptr;
