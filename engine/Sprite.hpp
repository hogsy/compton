#pragma once

class Sprite {
public:
    Sprite();
    Sprite(ALLEGRO_BITMAP *bitmap);
    Sprite(ALLEGRO_BITMAP *bitmap, float x, float y);
    virtual ~Sprite();

    ALLEGRO_BITMAP *GetBitmap() { return _bitmap; }
    virtual void SetBitmap(ALLEGRO_BITMAP *bitmap);

    virtual void Draw();

    bool InsideBounds();

    PLVector2D m_LocalPosition;
    PLVector2D m_Position;
    PLVector2D m_Origin;
    float m_Angle;

protected:
private:
    ALLEGRO_BITMAP *_bitmap;

    unsigned int framecount, currentframe;
    int framew, frameh;
    double framedelay;

    int _w, _h;
};