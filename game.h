// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

typedef struct GameVars {
	unsigned int state;
	unsigned int menu_state;

	ALLEGRO_BITMAP *menu_earth;

	ALLEGRO_FONT *font_title;
	ALLEGRO_FONT *font_small, *font_gothic_medium;
	ALLEGRO_FONT *font_chunk;
} GameVars;

extern GameVars game;

///////////////////////////////////////////

class SpriteObject
{
public:
	SpriteObject();
	SpriteObject(ALLEGRO_BITMAP *sprite);
	SpriteObject(ALLEGRO_BITMAP *sprite, float x, float y);

	virtual ~SpriteObject();

	ALLEGRO_BITMAP *GetSprite() { return _sprite; }
	void SetSprite(ALLEGRO_BITMAP *sprite);

	virtual void Draw();

	bool InsideBounds();

	Vector2D position;
	Vector2D origin;
	float angle;

protected:
private:
	ALLEGRO_BITMAP *_sprite;

	unsigned int 	framecount, currentframe;
	int 			framew, frameh;
	double 			framedelay;

	int _w, _h;
};