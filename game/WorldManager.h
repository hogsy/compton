// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

class CloudObject;
class MoonObject;
class Sun;

class WorldManager
{
public:
	WorldManager();
	~WorldManager();

	void Simulate();
	void Draw();

	unsigned int GetSecond()    { return _second; }
	unsigned int GetMinute()    { return _minute; }
	unsigned int GetHour()      { return _hour; }
	unsigned int GetDay()       { return _day; }
	unsigned int GetWeek()      { return 0; }
	unsigned int GetMonth()     { return _month; }
	unsigned int GetYear()      { return _year; }

	float GetWindSpeed() { return _wind_speed; }

	unsigned int GetTotalSeconds()  { return static_cast<unsigned int>(_time); }
	unsigned int GetTotalHours()    { return static_cast<unsigned int>(std::floor(_time / 60)); }
	unsigned int GetTotalDays()     { return static_cast<unsigned int>(std::floor(_time / 86400)); }
	unsigned int GetTotalWeeks()    { return static_cast<unsigned int>(std::floor(_time / 604800)); }
	unsigned int GetTotalMonths()   { return _month + ((_year - 1) * 12); } // Months are a special case...

	const char * GetDayString();

	ALLEGRO_BITMAP *cloud_droplet;

protected:
private:
	float _wind_speed;

	double _time;
	unsigned int _year, _month, _day, _hour, _minute, _second;

	ALLEGRO_COLOR _sky_top, _sky_bottom;
	ALLEGRO_COLOR _sky_target_top, _sky_target_bottom;

	ALLEGRO_BITMAP *_sky_background;

	MoonObject *_moon;
	Sun *_sun;

	float _temperature;

	std::vector<CloudObject> _clouds;
	std::vector<ALLEGRO_BITMAP*> _cloud_sprites;
	unsigned int _cloud_density;
};

extern WorldManager *game_worldmanager;
