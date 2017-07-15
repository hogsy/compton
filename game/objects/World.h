// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

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
    std::string GetName() { return m_Name; }

    unsigned int GetSecond() { return _second; }
    unsigned int GetMinute() { return _minute; }
    unsigned int GetHour() { return _hour; }
    unsigned int GetDay() { return m_Day; }
    unsigned int GetWeek() { return 0; }
    unsigned int GetMonth() { return m_Month; }
    unsigned int GetYear() { return m_Year; }

    float GetWindSpeed() { return m_WindSpeed; }

    unsigned int GetTotalSeconds() { return static_cast<unsigned int>(m_Time); }
    unsigned int GetTotalHours() { return static_cast<unsigned int>(std::floor(m_Time / 60)); }
    unsigned int GetTotalDays() { return static_cast<unsigned int>(std::floor(m_Time / 86400)); }
    unsigned int GetTotalWeeks() { return static_cast<unsigned int>(std::floor(m_Time / 604800)); }
    unsigned int GetTotalMonths() { return m_Month + ((m_Year - 1) * 12); } // Months are a special case...

    unsigned int GetWidth() { return m_WorldWidth; }
    unsigned int GetHeight() { return m_WorldHeight; }

    const char *GetDayString();
    const char *GetMonthString();

    ALLEGRO_BITMAP *cloud_droplet; // todo, move to a global table...

protected:
private:
    World();

    double m_Time;
    unsigned int m_Year, m_Month, m_Day, _hour, _minute, _second;

    ALLEGRO_COLOR m_SkyTop, m_SkyBottom;
    ALLEGRO_COLOR m_SkyTargetTop, m_SkyTargetBottom;
    ALLEGRO_BITMAP *m_SkyBackground;

    unsigned int m_WorldWidth, m_WorldHeight;

    Moon *m_Moon;
    Sun *m_Sun;

    float m_Temperature;
    float m_WindSpeed;

    std::string m_Name; // The name of the planet.

    std::vector<CloudObject> m_Clouds;
    std::vector<ALLEGRO_BITMAP *> m_CloudSprites;
    unsigned int _cloud_density;

public:
    static World *GetInstance() {
        if (!game_worldmanager) {
            game_worldmanager = new World();
        }

        return game_worldmanager;
    }
};
