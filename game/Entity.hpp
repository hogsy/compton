// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#pragma once

#include "../engine/Sprite.hpp"

class Entity {
public:
    Entity();
    ~Entity();

    virtual void SetPosition(PLVector2D position);
    virtual void SetRotation(float angle);

    virtual void Draw();

    void SetSprite(std::string path);

    Sprite *sprite;

protected:
private:
    PLVector2D position_;
    float angle_;
};
