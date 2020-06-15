// Virtual Critters, Copyright (C) 2016-2019 Mark Elsworth Sowden
//  GLORY TO THE MOON MEN!

#include "../../shared.h"

#include "object_world.h"
#include "object_moon.h"
#include "../game.h"

Moon::Moon() : Sprite(engine::LoadImage("environment/objects/moon")) {
    angle = 0;

    // Grab the x and y, then position us in the middle
    // of the screen.
    origin_.x = al_get_bitmap_width(GetBitmap()) / 2;
    origin_.y = 420;
    position_.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

    cycle_ = 0;
}

void Moon::Simulate() {
    angle = static_cast<float>(engine_vars.counter / 86400);
}

void Moon::Draw() {
    PLVector2 oldpos = position_;
    position_.x -= game.camera_x / 2;
    position_.y -= game.camera_y / 2;
    Sprite::Draw();
    position_ = oldpos;
}
