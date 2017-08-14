
#pragma once
#include "../../shared.h"
#include "../../engine/sprite.h"

class Creature;

class CreatureObject : public Sprite {
public:
    CreatureObject() : Sprite(engine::LoadImage("sprites")) {

    }

    void Draw() override;

    virtual void Simulate();

    bool is_grabbed{false};

    Creature *parent_{nullptr};

    PLVector2D velocity_;

protected:
    unsigned int s_x_{0}, s_y_{0};
    unsigned int s_w_{0}, s_h_{0};

private:

};

class CreatureDrink : public CreatureObject {
public:
    CreatureDrink() {
        s_x_ = 119; s_y_ = 118;
        s_w_ = 9;   s_h_ = 5;

        position_.Set(14.f, 0);
        origin_.Set(s_w_ / 2, s_h_ / 2);
    }

    void Simulate() override {
        CreatureObject::Simulate();


    }
protected:
private:
};

class CreatureToy : public CreatureObject {
public:
    CreatureToy() {
        s_x_ = 123; s_y_ = 123;
        s_w_ = 5;   s_h_ = 5;

        position_.Set(54.f, 0);
        origin_.Set(s_w_ / 2, s_h_ / 2);
    }

    void Simulate() override;
protected:
private:
};

////////////////////////////////////

class Creature : public Sprite {
public:
    enum {
        EMO_HAPPINESS,
        EMO_ANGER,
        EMO_INDIFFERENT,
        EMO_SADNESS,
        EMO_BOREDOM,

        EMO_END
    };

    enum {
        DIR_NONE,
        DIR_DRINK,
        DIR_PLAY,
        DIR_SLEEP,
        DIR_RAGE,
        DIR_SHOCKED,
    };

    enum {
        OCU_NONE,
        OCU_SLEEPING,
        OCU_DRINKING,
        OCU_PLAYING,
    };

    enum {
        LOO_NONE,
        LOO_CURSOR,
        LOO_OBJECT,
    };

    Creature();

    void SetState(unsigned int state) {
        old_state_ = state_;
        state_ = state;
    }

    unsigned int GetState() { return state_; }
    unsigned int GetThirst() { return static_cast<unsigned int>(thirst_); }
    unsigned int GetHealth() { return static_cast<unsigned int>(health_); }

    void Draw() override;
    void Simulate();

    void DropObject();
    void ThrowObject();
    bool PickupObject(CreatureObject *object);

    void Jump(float velocity);
    void Impact();

    void WakeUp();

    void CheckRage();

    void SetDirective(unsigned int dir);
    void SetOccupation(unsigned int ocu);

    void ClearLook() {
        target_look_ = LOO_NONE;
        look_object_ = nullptr;
    }

protected:
private:
    double emotions_[EMO_END];

    unsigned int state_, old_state_;
    unsigned int occupation_{OCU_NONE};
    unsigned int directive_{DIR_NONE};

    unsigned int target_look_{LOO_NONE};

    float thirst_, health_;

    double delay_look_{1000};
    double delay_movement;
    double delay_lastmove;
    double delay_play_, delay_throw_{1000};
    double delay_drink_;
    double delay_sleep_;

    CreatureObject *grabbed_object_{nullptr};
    CreatureObject *look_object_{nullptr};

    bool is_grabbed, is_grounded_{false};

    PLVector2D velocity_;
    PLVector2D old_position_;
};

extern CreatureDrink   *drink;
extern CreatureToy     *toy;
extern Creature        *creature;