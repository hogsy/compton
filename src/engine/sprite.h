/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

struct ALLEGRO_BITMAP;

class Sprite {
public:
    Sprite();
    explicit Sprite(ALLEGRO_BITMAP *bitmap);
    Sprite(ALLEGRO_BITMAP *bitmap, float x, float y);
    virtual ~Sprite();

    ALLEGRO_BITMAP *GetBitmap() { return bitmap_; }
    virtual void SetBitmap(ALLEGRO_BITMAP *bitmap);

    virtual void Draw();

    bool IsVisible();

    PLVector2 position_;
    PLVector2 origin_;
    float angle;

protected:
private:
    ALLEGRO_BITMAP *bitmap_;

    unsigned int frame_count_, current_frame_;
    int frame_w_, frame_h_;
    double frame_delay_;

    int _w, _h;
};