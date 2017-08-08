#pragma once

class Sprite {
public:
    Sprite();
    Sprite(ALLEGRO_BITMAP *bitmap);
    Sprite(ALLEGRO_BITMAP *bitmap, float x, float y);
    virtual ~Sprite();

    ALLEGRO_BITMAP *GetBitmap() { return bitmap_; }
    virtual void SetBitmap(ALLEGRO_BITMAP *bitmap);

    virtual void Draw();

    bool IsVisible();

    PLVector2D position_;
    PLVector2D origin_;
    float angle;

protected:
private:
    ALLEGRO_BITMAP *bitmap_;

    unsigned int frame_count_, current_frame_;
    int frame_w_, frame_h_;
    double frame_delay_;

    int _w, _h;
};