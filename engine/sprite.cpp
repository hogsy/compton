
#include "../shared.h"
#include "sprite.h"
#include "../game/game.h"

Sprite::Sprite() :
        bitmap_(nullptr), origin_(0, 0), position_(0, 0), angle(0), _w(0), _h(0),
        current_frame_(0), frame_count_(0), frame_delay_(0) {}

Sprite::Sprite(ALLEGRO_BITMAP *bitmap) :
        bitmap_(bitmap), origin_(0, 0), position_(0, 0), angle(0),
        current_frame_(0), frame_count_(0), frame_delay_(0) {
    frame_w_ = _w = al_get_bitmap_width(bitmap);
    frame_h_ = _h = al_get_bitmap_height(bitmap);
    origin_.Set(_w / 2, _h / 2);
}

Sprite::Sprite(ALLEGRO_BITMAP *bitmap, float x, float y) :
        bitmap_(bitmap), origin_(0, 0), position_(x, y), angle(0),
        current_frame_(0), frame_count_(0), frame_delay_(0) {
    frame_w_ = _w = al_get_bitmap_width(bitmap);
    frame_h_ = _h = al_get_bitmap_height(bitmap);
    origin_.Set(_w / 2, _h / 2);
}

Sprite::~Sprite() {
    //if(bitmap_) al_destroy_bitmap(bitmap_);
}

void Sprite::SetBitmap(ALLEGRO_BITMAP *bitmap) {
    if(!bitmap) {
        PRINT_ERROR("Attempted to pass an invalid bitmap to a sprite!\n");
    }

    bitmap_ = bitmap;
    _w = al_get_bitmap_width(bitmap);
    _h = al_get_bitmap_height(bitmap);
    origin_.Set(_w / 2, _h / 2);
}

void Sprite::Draw() {
    if(!IsVisible()) {
        return;
    }

    al_draw_scaled_rotated_bitmap(
            bitmap_,
            origin_.x, origin_.y,
            position_.x, position_.y,
            1, 1,
            angle,
            0
    );
}

bool Sprite::IsVisible() {
    PLVector2D cposition = position_ - origin_;
    if(((cposition.x + _w) < 0) || (cposition.x > DISPLAY_WIDTH)) {
        return false;
    }

    if(((cposition.y + _h) < 0) || (cposition.y > DISPLAY_HEIGHT)) {
        return false;
    }

    return true;
}
