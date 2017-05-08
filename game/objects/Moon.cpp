// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden
//  GLORY TO THE MOON MEN!

#include "../../Shared.h"

#include "World.h"
#include "Moon.h"
#include "../game.h"

Moon::Moon() : Sprite(engine::LoadImage("environment/objects/moon")) {
    m_Angle = 0;

    // Grab the x and y, then m_LocalPosition us in the middle
    // of the screen.
    m_Origin.x = al_get_bitmap_width(GetBitmap()) / 2;
    m_Origin.y = 420;
    m_LocalPosition.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

    cycle_ = 0;
}

void Moon::Simulate() {
    m_Angle = engine_vars.counter / 86400;
}

void Moon::Draw() {
    PLVector2D oldpos = m_LocalPosition;
    m_LocalPosition.x -= game.camera_x / 2;
    m_LocalPosition.y -= game.camera_y / 2;
    Sprite::Draw();
    m_LocalPosition = oldpos;
}
