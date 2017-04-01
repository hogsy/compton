#pragma once

#include "../../engine/Sprite.hpp"

// todo, need to do the difference phases of the moon etc.
class Moon : public Sprite {
public:
    Moon();
    ~Moon() {}

    void Simulate();

    virtual void Draw();

protected:
private:
    unsigned int cycle_;
};