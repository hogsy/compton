
#include "shared.h"

#include "game.h"
#include "world.h"
#include "object_moon.h"

//  GLORY TO THE MOON MEN!

Moon::Moon() : SpriteObject(LoadImage("environment/objects/moon")) {
    angle = 0;

    // Grab the x and y, then position us in the middle
    // of the screen.
    origin.x = al_get_bitmap_width(GetSprite()) / 2;
    origin.y = 420;
    position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

    cycle_ = 0;
}

void Moon::Simulate() {
    angle = game_skymanager->GetTotalSeconds() * 60 / 360;
}
