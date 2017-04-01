// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../Shared.h"
#include "Entity.hpp"
#include "game.h"

Entity::Entity() {

}

Entity::~Entity() {
    if(sprite) {
        delete sprite;
    }
}

void Entity::SetSprite(std::string path) {
    if(sprite) {
        delete sprite;
    }

    
}

void Entity::SetPosition(PLVector2D position) {
    if(position == position_) {
        return;
    }

    sprite->position = position;
}

void Entity::SetRotation(float angle) {
    if(angle == angle_) {
        return;
    }

    sprite->angle = angle;
}

void Entity::Draw() {
    sprite->position.x = position_.x - game.camera_x;
    sprite->position.y = position_.y - game.camera_y;
    sprite->Draw();
}
