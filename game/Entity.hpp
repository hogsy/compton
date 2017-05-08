// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

#include "../engine/Sprite.hpp"

class Entity : public Sprite {
public:
    Entity();
    ~Entity();

    virtual void SetPosition(PLVector2D position);
    virtual void SetRotation(float angle);

    virtual void Draw();
    virtual void Simulate();

    virtual bool IsInteractive() { return false; }
};
