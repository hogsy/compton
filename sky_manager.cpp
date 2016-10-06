// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

#include "game.h"
#include "sky_manager.h"

SkyManager *game_skymanager = nullptr;

/*	Clouds	*/

#define DAMP 	((w * h) / ((float)plGenerateUniformRandom(0.05f) + 5)) / 100;
#define JIGGLE	0.05f

class Cloud : public SpriteObject
{
public:
	Cloud(ALLEGRO_BITMAP *sprite) : SpriteObject(sprite)
	{
		w = al_get_bitmap_width(sprite);
		h = al_get_bitmap_height(sprite);

		position.Set(rand()%DISPLAY_WIDTH, rand()%(DISPLAY_HEIGHT/2));
		_damping = DAMP;
		_jiggle = (float)plGenerateUniformRandom(JIGGLE);
	}

	Cloud(ALLEGRO_BITMAP *sprite, bool direction) : SpriteObject(sprite)
	{
		w = al_get_bitmap_width(sprite);
		h = al_get_bitmap_height(sprite);

		position.Set((direction ? -(float)w : DISPLAY_WIDTH ), rand()%(DISPLAY_HEIGHT / 2));
		_damping = DAMP;
		_jiggle = (float)plGenerateUniformRandom(JIGGLE);
	}

	virtual ~Cloud()
	{
		SetSprite(nullptr);
	}

	void Move(float speed)
	{
		position.x += speed / _damping;
		static double s = rand()%100;
		angle = std::cos((float)s++ / 1000) * 0.05f;
	}

	virtual void Draw()
	{
		PLVector2D oldpos = position;
		position.y = (std::sin((float)engine.counter / (120 / _jiggle)) * 5 + 5) + position.y;
		SpriteObject::Draw();
		position = oldpos;
	}

	int w, h;

protected:
private:
	float _damping, _jiggle;
};

#define ROTATION 0.01f	//0.0001f

class Moon : public SpriteObject
{
public:
	Moon() : SpriteObject(LoadImage("moon"))
	{
		origin.x = al_get_bitmap_width(GetSprite()) / 2;
		origin.y = 420;
		position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

		angle = 0;
	}

	void Simulate()
	{
		angle += ROTATION;
	}

protected:
private:
};

class Sun : public SpriteObject
{
public:
	Sun() : SpriteObject(LoadImage("sun"))
	{
		origin.x = al_get_bitmap_width(GetSprite()) / 2;
		origin.y = -420;
		position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

		angle = 0;
	}

	void Simulate()
	{
		angle += ROTATION;
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef struct SkyColourCycle
{
	ALLEGRO_COLOR top, bottom;

	unsigned int hour;
} SkyColourCycle;

SkyColourCycle sky_colourcycle[]=
{
		// 	TOP					BOTTOM
		{	{ 20, 20, 20 }, 	{ 0.33, 85, 85 },	0	},	// MIDNIGHT
		{ 	{ 29, 38, 55 },		{ 0.96, 140, 72 },	6	},	// SUNRISE
		{	{ 118, 139, 156 },	{ 0.96, 167, 80 },	12	},	// MIDDAY
		{	{ 99, 166, 253 },	{ 0.87, 142, 109 },	18	},	// SUNSET
};

std::string sky_days[]= {
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday",
		"Sunday"
};

/*	todo
 * 	leap year stuff... formula
 * 		https://www.timeanddate.com/date/leapyear.html
 */

#define WIND_SPEED 10.5f

SkyManager::SkyManager() : _wind_speed(WIND_SPEED), _time(0), _day(0), _hour(0), _minute(0), _second(0)
{
	_cloud_sprites.reserve(10);
	for(int i = 0; i < 10; i++)
		_cloud_sprites[i] = LoadImage("clouds/" + std::to_string(i));

	_sky_top 			=  //sky_colourcycle[0].top;
	_sky_target_top 	= //sky_colourcycle[1].top;
	_sky_bottom 		= //sky_colourcycle[0].bottom;
	_sky_target_bottom 	= al_map_rgb(0, 0, 0); //sky_colourcycle[1].bottom;
	_sky_background		= LoadImage("night_background");

	// Make initial set of clouds.
	for(int i = 0; i < 8; i++)
	{
		ALLEGRO_BITMAP *sprite = _cloud_sprites[rand()%10];
		_clouds.emplace(_clouds.end(), Cloud(sprite));
	}

	_moon = new Moon();
	_sun = new Sun();
}

SkyManager::~SkyManager()
{
	for(int i = 0; i < _cloud_sprites.size(); i++)
		al_destroy_bitmap(_cloud_sprites[i]);
	_cloud_sprites.clear();

	delete _moon;
}

void SkyManager::Simulate()
{
	if(_clouds.size() < 8)
	{
		ALLEGRO_BITMAP *sprite = _cloud_sprites[rand()%10];
		_clouds.emplace(
				_clouds.end(),
				Cloud(
						sprite,
						(bool)_wind_speed
				)
		);
	}

#if 0
	static int delay = 0;
		delay++; if(delay < 5) return;
		delay = 0;
#endif

	for(unsigned int i = 0; i < _clouds.size(); ++i)
	{
		_clouds[i].Move(_wind_speed);
		if(!_clouds[i].InsideBounds())
			_clouds.erase(_clouds.begin() + i);
	}

	static double difference = 1;

	_time++; _second = 70; //++;
	if(_second > 60) { _minute += 1; _second = 0; }
	if(_minute > 60)
	{
		_hour += 1; _minute = 0; _second = 0;
		for(int i = 0; i < 4; i++)
		{
			if(_hour >= sky_colourcycle[i].hour)
			{
				_sky_target_top 	= sky_colourcycle[i].top;
				_sky_target_bottom 	= sky_colourcycle[i].bottom;

				difference = sky_colourcycle[i].hour / _hour;
			}
		}
	}
	if(_hour > 24) { _day += 1; _hour = 0; _minute = 0; _second = 0; }
	if(_day > 6) { _day = 0; _hour = 0; _minute = 0; _second = 0; }

	// Sky gradient
	//int difference =
#define INTERP (float)((_time / 60) / 4)

	_sky_bottom.r = plCosineInterpolate(
			_sky_bottom.r, _sky_target_bottom.r,
			INTERP
	);
	_sky_bottom.g = plCosineInterpolate(
			_sky_bottom.g, _sky_target_bottom.g,
			INTERP
	);
	_sky_bottom.b = plCosineInterpolate(
			_sky_bottom.b, _sky_target_bottom.b,
			INTERP
	);

#if 1
	_sky_top.r = plCosineInterpolate(
			_sky_top.r, _sky_target_top.r,
			INTERP
	);
	_sky_top.g = plCosineInterpolate(
			_sky_top.g, _sky_target_top.g,
			INTERP
	);
	_sky_top.b = plCosineInterpolate(
			_sky_top.b, _sky_target_top.b,
			INTERP
	);
#endif

	_moon->Simulate();
	_sun->Simulate();

	printf("(%f) %f\n", _time, _sky_bottom.r);
}

void SkyManager::Draw()
{
#if 0
	DrawVerticalGradientRectangle(
			0, 0,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			_sky_top, _sky_bottom
	);
#endif

	static float alpha = 0;
	if(_hour < 6 || _hour > 18) {
		if(_hour) if(alpha < 0.5f) alpha += 0.01f;
		al_draw_tinted_bitmap(_sky_background, al_map_rgba_f(1, 1, 1, alpha), 0, 0, 0);
	} else alpha = 0;

	_moon->Draw();
	_sun->Draw();

	// Clouds
	DrawBitmap(
			_cloud_sprites[2],
			10, 10,
			al_get_bitmap_width(_cloud_sprites[2]),
			al_get_bitmap_height(_cloud_sprites[2])
	);
	for(Cloud &cloud : _clouds)	cloud.Draw();

	// Time counter
	DrawString(game.font_small, 20, 420, al_map_rgb(255, 255, 255), sky_days[_day]);
	char timestr[10] = { 0 };
	snprintf(timestr, 10, "%02u:%02u:%02u", _hour, _minute, _second);
	DrawString(game.font_small, 20, 440, al_map_rgb(255, 255, 255), timestr);
}
