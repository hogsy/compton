
#include "Shared.h"

#include "game.h"
#include "WorldManager.h"
#include "MoonObject.h"

//  GLORY TO THE MOON MEN!

MoonObject::MoonObject() : SpriteObject(LoadImage("environment/objects/moon")) {
    angle = 0;

    // Grab the x and y, then position us in the middle
    // of the screen.
    origin.x = al_get_bitmap_width(GetSprite()) / 2;
    origin.y = 420;
    position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

    cycle_ = 0;
}

void MoonObject::Simulate() {
    angle = game_skymanager->GetTotalSeconds() * 60 / 360;
}
