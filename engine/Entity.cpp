// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../Shared.h"
#include "Entity.hpp"
#include "../game/game.h"

Entity::Entity() : Sprite(game.entity_icon) {
    m_Angle = 0;
    m_LocalPosition.Set(0, 0);
}

Entity::~Entity() {
}

void Entity::SetPosition(PLVector2D position) {
    m_LocalPosition = position;
}

void Entity::SetRotation(float angle) {
    m_Angle = angle;
}

void Entity::Draw() {
    // Transform for camera m_LocalPosition...
    PLVector2D oposition = m_LocalPosition;
    m_LocalPosition.x -= game.camera_x;
    m_LocalPosition.y -= game.camera_y;

    if(!InsideBounds()) {
        return;
    }

    Sprite::Draw();

    // Restore the original m_LocalPosition...
    m_LocalPosition = oposition;
}

void Entity::Simulate() {

}
