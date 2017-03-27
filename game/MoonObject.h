#pragma once

#include "Sprite.hpp"

// todo, need to do the difference phases of the moon etc.
class MoonObject : public Sprite {
public:
    MoonObject();
    ~MoonObject() {}

    void Simulate();

protected:
private:
    unsigned int cycle_;
};