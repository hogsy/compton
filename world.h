// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

class Cloud;
class Moon;
class Sun;

class WorldManager
{
public:
	WorldManager();
	~WorldManager();

	void Simulate();
	void Draw();

	unsigned int GetSecond() { return _second; }
	unsigned int GetMinute() { return _minute; }
	unsigned int GetHour() { return _hour; }
	unsigned int GetDay() { return _day; }

	float GetWindSpeed() { return _wind_speed; }

	unsigned int GetTotalHours() { }
	// Get the total number of days...
	// We can't do this by month, because months aren't always the same length
	// We know there's 365 days in a year...
	unsigned int GetTotalDays() { return 0; }
	unsigned int GetTotalMonths() { return _month + ((_year - 1) * 12); }

	std::string GetDayString();

	ALLEGRO_BITMAP *cloud_droplet;

protected:
private:
	float _wind_speed;

	double _time;
	unsigned int _year, _month, _day, _hour, _minute, _second;

	ALLEGRO_COLOR _sky_top, _sky_bottom;
	ALLEGRO_COLOR _sky_target_top, _sky_target_bottom;

	ALLEGRO_BITMAP *_sky_background;

	Moon *_moon;
	Sun *_sun;

	float _temperature;

	std::vector<Cloud> _clouds;
	std::vector<ALLEGRO_BITMAP*> _cloud_sprites;
	unsigned int _cloud_density;
};

extern WorldManager *game_skymanager;
