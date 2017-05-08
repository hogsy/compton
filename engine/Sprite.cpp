
#include "../Shared.h"
#include "Sprite.hpp"
#include "../game/game.h"

Sprite::Sprite() :
        _bitmap(nullptr), m_Origin(0, 0), m_LocalPosition(0, 0), m_Angle(0), _w(0), _h(0),
        currentframe(0), framecount(0), framedelay(0) {}

Sprite::Sprite(ALLEGRO_BITMAP *bitmap) :
        _bitmap(bitmap), m_Origin(0, 0), m_LocalPosition(0, 0), m_Angle(0),
        currentframe(0), framecount(0), framedelay(0) {
    framew = _w = al_get_bitmap_width(bitmap);
    frameh = _h = al_get_bitmap_height(bitmap);
    m_Origin.Set(_w / 2, _h / 2);
}

Sprite::Sprite(ALLEGRO_BITMAP *bitmap, float x, float y) :
        _bitmap(bitmap), m_Origin(0, 0), m_LocalPosition(x, y), m_Angle(0),
        currentframe(0), framecount(0), framedelay(0) {
    framew = _w = al_get_bitmap_width(bitmap);
    frameh = _h = al_get_bitmap_height(bitmap);
    m_Origin.Set(_w / 2, _h / 2);
}

Sprite::~Sprite() {
    //if(_bitmap) al_destroy_bitmap(_bitmap);
}

void Sprite::SetBitmap(ALLEGRO_BITMAP *bitmap) {
    if(!bitmap) std::runtime_error("Attempted to pass an invalid bitmap to a sprite!\n");
    _bitmap = bitmap;
    _w = al_get_bitmap_width(bitmap);
    _h = al_get_bitmap_height(bitmap);
    m_Origin.Set(_w / 2, _h / 2);
}

void Sprite::Draw() {
    al_draw_scaled_rotated_bitmap(
            _bitmap,
            m_Origin.x, m_Origin.y,
            m_LocalPosition.x, m_LocalPosition.y,
            1, 1,
            m_Angle,
            0
    );
}

bool Sprite::InsideBounds() {
#if 0
    if (m_LocalPosition.x < -(float) _w || m_LocalPosition.x > DISPLAY_WIDTH + _w)
        return false;
#else // new position relative to camera transform
    if((m_LocalPosition.x + (float)(_w)) < game.camera_x) {
        return false;
    } else if(m_LocalPosition.x > (game.camera_x + (float)(engine_vars.window_width))) {
        return false;
    } else if((m_LocalPosition.y + (float)(_h)) < game.camera_y) {
        return false;
    } else if(m_LocalPosition.y > (game.camera_y + (float)(engine_vars.window_height))) {
        return false;
    }
#endif

    return true;
}
