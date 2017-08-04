// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

#include "sprite.h"

class Entity : public Sprite {
public:
    Entity();
    ~Entity();

    virtual void SetPosition(PLVector2D position);
    virtual void SetRotation(float angle);

    virtual void Draw();
    virtual void Simulate();

    virtual bool IsInteractive() { return false; }

protected:
private:
    PLVector2D _global_position;
};
