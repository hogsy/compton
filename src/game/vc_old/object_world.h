/*
SimGame Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

#include "object_creature.h"

class CloudObject;
class Moon;
class Sun;
class World;

extern World *game_worldmanager;

class World
{
public:
	~World();

	void Tick();
	void Draw();

	void		SetName( std::string name );
	std::string GetName() { return name_; }

	unsigned int GetSecond() { return _second; }
	unsigned int GetMinute() { return _minute; }
	unsigned int GetHour() { return _hour; }
	unsigned int GetDay() { return _day; }
	unsigned int GetWeek() { return 0; }
	unsigned int GetMonth() { return m_Month; }
	unsigned int GetYear() { return year_; }

	float GetWindSpeed() { return wind_speed_; }

	unsigned int GetTotalSeconds() { return static_cast< unsigned int >( time_ ); }
	unsigned int GetTotalHours() { return static_cast< unsigned int >( std::floor( time_ / 60 ) ); }
	unsigned int GetTotalDays() { return static_cast< unsigned int >( std::floor( time_ / 86400 ) ); }
	unsigned int GetTotalWeeks() { return static_cast< unsigned int >( std::floor( time_ / 604800 ) ); }
	unsigned int GetTotalMonths() { return m_Month + ( ( year_ - 1 ) * 12 ); }// Months are a special case...

	unsigned int GetWidth() { return width_; }
	unsigned int GetHeight() { return height_; }

	const char *GetDayString();
	const char *GetMonthString();

protected:
private:
	World();

	double		 time_;
	unsigned int year_, m_Month, _day, _hour, _minute, _second;

	ALLEGRO_COLOR	sky_top_{}, sky_bottom_{};
	ALLEGRO_COLOR	sky_toptarget_{}, sky_bottomtarget_{};
	ALLEGRO_BITMAP *sky_background_;

	unsigned int width_, height_;

	//  Moon *moon_;
	//  Sun *sun_;

	float temperature_;
	float wind_speed_;

	std::string name_;// The name of the planet.

	std::vector< CloudObject >		m_Clouds;
	std::vector< ALLEGRO_BITMAP * > cloud_sprites_;
	unsigned int					cloud_density_;

public:
	static World *Get()
	{
		if ( !game_worldmanager )
		{
			game_worldmanager = new World();
		}

		return game_worldmanager;
	}
};
