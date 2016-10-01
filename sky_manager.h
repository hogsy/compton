// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

class Cloud;
class Moon;
class Sun;

class SkyManager
{
public:
	SkyManager();
	~SkyManager();

	void Simulate();
	void Draw();

	unsigned int GetSecond() { return _second; }
	unsigned int GetMinute() { return _minute; }
	unsigned int GetHour() { return _hour; }
	unsigned int GetDay() { return _day; }

protected:
private:
	float _wind_speed;

	double _time;
	unsigned int _day, _hour, _minute, _second;

	ALLEGRO_COLOR _sky_top, _sky_bottom;
	ALLEGRO_COLOR _sky_target_top, _sky_target_bottom;

	ALLEGRO_BITMAP *_sky_background;

	Moon *_moon;
	Sun *_sun;

	std::vector<Cloud> _clouds;
	std::vector<ALLEGRO_BITMAP*> _cloud_sprites;
};

extern SkyManager *game_skymanager;
