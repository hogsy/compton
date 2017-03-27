
#include "../Shared.h"
#include "Sprite.hpp"

Sprite::Sprite() :
        _sprite(nullptr), origin(0, 0), position(0, 0), angle(0), _w(0), _h(0),
        currentframe(0), framecount(0), framedelay(0) {}

Sprite::Sprite(ALLEGRO_BITMAP *sprite) :
        _sprite(sprite), origin(0, 0), position(0, 0), angle(0),
        currentframe(0), framecount(0), framedelay(0) {
    framew = _w = al_get_bitmap_width(sprite);
    frameh = _h = al_get_bitmap_height(sprite);
    origin.Set(_w / 2, _h / 2);
}

Sprite::Sprite(ALLEGRO_BITMAP *sprite, float x, float y) :
        _sprite(sprite), origin(0, 0), position(x, y), angle(0),
        currentframe(0), framecount(0), framedelay(0) {
    framew = _w = al_get_bitmap_width(sprite);
    frameh = _h = al_get_bitmap_height(sprite);
    origin.Set(_w / 2, _h / 2);
}

Sprite::~Sprite() {
    //if(_sprite) al_destroy_bitmap(_sprite);
}

void Sprite::SetSprite(ALLEGRO_BITMAP *sprite) {
    _sprite = sprite;
    if (_sprite) {
        _w = al_get_bitmap_width(sprite);
        _h = al_get_bitmap_height(sprite);
    }
}

void Sprite::Draw() {
    if (!InsideBounds()) return;
    al_draw_scaled_rotated_bitmap(
            _sprite,
            origin.x, origin.y,
            position.x, position.y,
            1, 1,
            angle,
            0
    );
}

bool Sprite::InsideBounds() {
    if (position.x < -(float) _w || position.x > DISPLAY_WIDTH + _w)
        return false;

    return true;
}
