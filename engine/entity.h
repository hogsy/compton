// Virtual Critters, Copyright (C) 2016-2019 Mark Elsworth Sowden

#pragma once

#include "sprite.h"

class Entity : public Sprite {
public:
    Entity();
    ~Entity() override;

    virtual void SetPosition(PLVector2 position);
    virtual void SetRotation(float angle);

    void Draw() override;
    virtual void Simulate();

    virtual bool IsInteractive() { return false; }

protected:
private:
    PLVector2 _global_position;
};
