// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../shared.h"
#include "entity.h"
#include "../game/game.h"

Entity::Entity() : Sprite(game.entity_icon) {
    angle = 0;
    position_.Set(0, 0);
    _global_position = position_;
}

Entity::~Entity() = default;

void Entity::SetPosition(PLVector2D position) {
    this->position_ = position;
}

void Entity::SetRotation(float angle) {
    angle = angle;
}

void Entity::Draw() {
    // Transform for camera position...
    PLVector2D oposition = position_;
    position_.x -= game.camera_x;
    position_.y -= game.camera_y;

    Sprite::Draw();

    // Restore the original position...
    position_ = oposition;
}

void Entity::Simulate() {

}
