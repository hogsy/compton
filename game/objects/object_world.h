// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

#include "object_creature.h"

class CloudObject;

class Moon;

class Sun;

class World;

extern World *game_worldmanager;

class World {
public:
    ~World();

    void Simulate();
    void Draw();

    void SetName(std::string name);
    std::string GetName() { return name_; }

    unsigned int GetSecond() { return _second; }
    unsigned int GetMinute() { return _minute; }
    unsigned int GetHour() { return _hour; }
    unsigned int GetDay() { return _day; }
    unsigned int GetWeek() { return 0; }
    unsigned int GetMonth() { return m_Month; }
    unsigned int GetYear() { return year_; }

    float GetWindSpeed() { return wind_speed_; }

    unsigned int GetTotalSeconds() { return static_cast<unsigned int>(time_); }
    unsigned int GetTotalHours() { return static_cast<unsigned int>(std::floor(time_ / 60)); }
    unsigned int GetTotalDays() { return static_cast<unsigned int>(std::floor(time_ / 86400)); }
    unsigned int GetTotalWeeks() { return static_cast<unsigned int>(std::floor(time_ / 604800)); }
    unsigned int GetTotalMonths() { return m_Month + ((year_ - 1) * 12); } // Months are a special case...

    unsigned int GetWidth() { return width_; }
    unsigned int GetHeight() { return height_; }

    const char *GetDayString();
    const char *GetMonthString();

    ALLEGRO_BITMAP *cloud_droplet; // todo, move to a global table...

protected:
private:
    World();

    double time_;
    unsigned int year_, m_Month, _day, _hour, _minute, _second;

    ALLEGRO_COLOR sky_top_, sky_bottom_;
    ALLEGRO_COLOR sky_toptarget_, sky_bottomtarget_;
    ALLEGRO_BITMAP *sky_background_;

    unsigned int width_, height_;

  //  Moon *moon_;
  //  Sun *sun_;

    float temperature_;
    float wind_speed_;

    std::string name_; // The name of the planet.

    std::vector<CloudObject> m_Clouds;
    std::vector<ALLEGRO_BITMAP *> cloud_sprites_;
    unsigned int cloud_density_;

    Creature *creature_;

public:
    static World *GetInstance() {
        if (!game_worldmanager) {
            game_worldmanager = new World();
        }

        return game_worldmanager;
    }
};
