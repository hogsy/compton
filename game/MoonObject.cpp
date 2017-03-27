// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden
//  GLORY TO THE MOON MEN!

#include "../Shared.h"

#include "WorldManager.h"
#include "MoonObject.h"

MoonObject::MoonObject() : Sprite(engine::LoadImage("environment/objects/moon")) {
    angle = 0;

    // Grab the x and y, then position us in the middle
    // of the screen.
    origin.x = al_get_bitmap_width(GetSprite()) / 2;
    origin.y = 420;
    position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

    cycle_ = 0;
}

void MoonObject::Simulate() {
    angle = game_worldmanager->GetHour();
}
