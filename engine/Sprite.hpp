#pragma once

class Sprite {
public:
    Sprite();
    Sprite(ALLEGRO_BITMAP *sprite);
    Sprite(ALLEGRO_BITMAP *sprite, float x, float y);
    virtual ~Sprite();

    ALLEGRO_BITMAP *GetSprite() { return _sprite; }

    void SetSprite(ALLEGRO_BITMAP *sprite);

    virtual void Draw();

    bool InsideBounds();

    PLVector2D position;
    PLVector2D origin;
    float angle;

protected:
private:
    ALLEGRO_BITMAP *_sprite;

    unsigned int framecount, currentframe;
    int framew, frameh;
    double framedelay;

    int _w, _h;
};