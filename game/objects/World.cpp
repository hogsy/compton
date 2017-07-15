// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../../Shared.h"

#include "../game.h"
#include "World.h"
#include "Moon.h"
#include "../../engine/Entity.hpp"

World *game_worldmanager = nullptr;

#define WORLD_NIGHT_START   18  // Starting hour for night to roll in.
#define WORLD_NIGHT_END     6   // Final hour that night starts rolling out.

/*	Clouds	*/

#define DAMP    ((w * h) / ((float)plGenerateUniformRandom(0.05f) + 5)) / 100;
#define JIGGLE  0.05f

class RainObject : public Sprite {
public:
    RainObject(PLVector2D pos) : Sprite(World::GetInstance()->cloud_droplet) {
        m_LocalPosition = pos;
    }

    void Simulate() {
        if (!InsideBounds()) {
            delete this;
        }
        m_LocalPosition.x += 0.5f;
        m_LocalPosition.y += (World::GetInstance()->GetWindSpeed() / 10.0f);
        m_Angle = -(World::GetInstance()->GetWindSpeed() / 100.0f);
    }

protected:
private:
};

class CloudObject : public Sprite {
public:
    CloudObject(ALLEGRO_BITMAP *sprite) : Sprite(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        m_LocalPosition.Set(rand() % DISPLAY_WIDTH, rand() % (DISPLAY_HEIGHT / 2));
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    CloudObject(ALLEGRO_BITMAP *sprite, bool direction) : Sprite(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        m_LocalPosition.Set((direction ? -(float) w : DISPLAY_WIDTH), rand() % (DISPLAY_HEIGHT / 2));
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    void Move(float speed) {
        m_LocalPosition.x += speed / _damping;
        static double s = rand() % 100;
        m_Angle = std::cos((float) s++ / 1000) * 0.05f;
    }

    virtual void Draw() {
        PLVector2D oldpos = m_LocalPosition;
        m_LocalPosition.y = (std::sin((float) engine_vars.counter / (120 / _jiggle)) * 5 + 5) + m_LocalPosition.y;

        m_LocalPosition.x -= game.camera_x;
        m_LocalPosition.y -= game.camera_y;

        Sprite::Draw();
        m_LocalPosition = oldpos;
    }

    int w, h;

protected:
private:
    float _damping, _jiggle;
};

#include "Moon.h"

class Sun : public Sprite {
public:
    Sun() : Sprite(engine::LoadImage("environment/objects/sun")) {
        m_Origin.x = al_get_bitmap_width(GetBitmap()) / 2;
        m_Origin.y = -420;
        m_LocalPosition.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

        m_Angle = 0;
    }

    void Simulate() {
        m_Angle = World::GetInstance()->GetTotalSeconds() * 60 / 360;
    }
};

////////////////////////////////////////////////////////////////////////////////

class EnvironmentBackground {
public:
    EnvironmentBackground() {
        plScanDirectory("./bin/sprites/environment/backgrounds/", "night.png",
                        EnvironmentBackground::LoadNightBackground);

        current_background = nullptr;
    }

    static void LoadNightBackground(PLchar *path) {
        printf("FOUND BACKGROUND LEL!\n");

        std::string properpath = path + 6;

    }

    void AddNightBackground(ALLEGRO_BITMAP *bitmap) {
        night.push_back(bitmap);
        if (night[night.size()]) {}
    }

    void Draw() {
        // Bleh, but only do this if a background is set...
        if (!current_background) return;

#if 0
        al_draw_tinted_scaled_rotated_bitmap(
                current_background,
                al_map_rgb(255, 255, 255),
                256, 256,
                DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2,




                1024, 1024,
                (float) engine.counter / 100,
                0
        );
#else
        al_draw_bitmap(
                current_background,
                0, 0,
                0
        );
#endif
    }

    void Simulate() {

    }

protected:
private:

    std::vector<ALLEGRO_BITMAP *> night;
    std::vector<ALLEGRO_BITMAP *> day;

    ALLEGRO_BITMAP *current_background;
};

EnvironmentBackground *env_background;

typedef struct SkyColourCycle {
    ALLEGRO_COLOR top, bottom;

    unsigned int hour;
} SkyColourCycle;

SkyColourCycle sky_colourcycle[] =
        {
                // 	TOP					BOTTOM
                {{20,  20,  20},  {0.33, 85,  85},  0},    // MIDNIGHT
                {{29,  38,  55},  {0.96, 140, 72},  6},    // SUNRISE
                {{118, 139, 156}, {0.96, 167, 80},  12},    // MIDDAY
                {{99,  166, 253}, {0.87, 142, 109}, 18},    // SUNSET
        };

const char *sky_days[] = {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
};

typedef struct Month {
    unsigned int start;

    const char *name;
} Month;

Month months[] = {
        {0, "Aurora"},      // Dawn
        {75, "Febriarius"},
        {150, "Martius"},
        {225, "Aprilis"},
        {300, "Umbra"},     // Darkness
};

/*	todo
 * 	leap year stuff... formula
 * 		https://www.timeanddate.com/date/leapyear.html
 */

#define WIND_SPEED 10.5f

#define CLOUD_BITMAPS 3

#include "PlanetNames.h"

World::World() :
        m_WindSpeed(WIND_SPEED),
        m_Time(0), m_Year(0), m_Month(0), m_Day(0), _hour(0), _minute(0), _second(0),
        m_WorldWidth(4000), m_WorldHeight(4000),
        _cloud_density(8),
        m_Temperature(20) {
    env_background = new EnvironmentBackground();

    m_Name = planet_names[rand() % plArrayElements(planet_names)];

    m_CloudSprites.reserve(10);
    for (int i = 0; i < CLOUD_BITMAPS; i++) {
        m_CloudSprites[i] = engine::LoadImage(std::string("clouds/" + std::to_string(i)).c_str());
    }

    m_SkyTop =  sky_colourcycle[0].top;
    m_SkyTargetTop = sky_colourcycle[1].top;
    m_SkyBottom = sky_colourcycle[0].bottom;
    m_SkyTargetBottom = al_map_rgb(0, 0, 0); //sky_colourcycle[1].bottom;
    m_SkyBackground = engine::LoadImage("environment/backgrounds/00night");

    cloud_droplet = engine::LoadImage("environment/objects/rain");

    // Make initial set of clouds.
    for (int i = 0; i < _cloud_density; i++) {
        ALLEGRO_BITMAP *sprite = m_CloudSprites[rand() % CLOUD_BITMAPS];
        m_Clouds.emplace(m_Clouds.end(), CloudObject(sprite));
    }

    m_Moon = new Moon();
    m_Sun = new Sun();
}

World::~World() {
    for (int i = 0; i < m_CloudSprites.size(); i++)
        al_destroy_bitmap(m_CloudSprites[i]);
    m_CloudSprites.clear();

    delete m_Moon;
}

const char *World::GetDayString() {
    return sky_days[m_Day];
}

const char *World::GetMonthString() {
    return months[m_Month].name;
}

void World::Simulate() {
    // CloudObject generation...
    if (m_Clouds.size() < _cloud_density) {
        ALLEGRO_BITMAP *sprite = m_CloudSprites[rand() % CLOUD_BITMAPS];
        m_Clouds.emplace(
                m_Clouds.end(),
                CloudObject(
                        sprite,
                        (bool) m_WindSpeed
                )
        );
    }

    for (unsigned int i = 0; i < m_Clouds.size(); ++i) {
        m_Clouds[i].Move(m_WindSpeed);
        if (!m_Clouds[i].InsideBounds())
            m_Clouds.erase(m_Clouds.begin() + i);
    }

    static double difference = 1;

    m_Time++;
    _second++;
    if (_second > 60) {
        _minute += 1;
        _second = 0;
    }
    if (_minute > 60) {
        _hour += 1;
        _minute = 0;
        _second = 0;
        for (int i = 0; i < 4; i++) {
            if (_hour >= sky_colourcycle[i].hour) {
                m_SkyTargetTop = sky_colourcycle[i].top;
                m_SkyTargetBottom = sky_colourcycle[i].bottom;

                difference = sky_colourcycle[i].hour / _hour;
            }
        }
    }
    if (_hour > 24) {
        m_Day += 1;
        _hour = 0;
        _minute = 0;
        _second = 0;
    }
    if (m_Day > 6) {
        m_Day = 0;
        _hour = 0;
        _minute = 0;
        _second = 0;
    }

    // Sky gradient
    //int difference =
#define INTERP (float)((m_Time / 60) / 4)
    // BOTTOM
    m_SkyBottom.r = plCosineInterpolate(
            m_SkyBottom.r, m_SkyTargetBottom.r,
            INTERP
    );
    m_SkyBottom.g = plCosineInterpolate(
            m_SkyBottom.g, m_SkyTargetBottom.g,
            INTERP
    );
    m_SkyBottom.b = plCosineInterpolate(
            m_SkyBottom.b, m_SkyTargetBottom.b,
            INTERP
    );
    // TOP
    m_SkyTop.r = plCosineInterpolate(
            m_SkyTop.r, m_SkyTargetTop.r,
            INTERP
    );
    m_SkyTop.g = plCosineInterpolate(
            m_SkyTop.g, m_SkyTargetTop.g,
            INTERP
    );
    m_SkyTop.b = plCosineInterpolate(
            m_SkyTop.b, m_SkyTargetTop.b,
            INTERP
    );

    m_Moon->Simulate();
    m_Sun->Simulate();
}

void World::Draw() {
    if(game.menu_state != GAME_MENU_DEFAULT) {
        return;
    }

#if 1
    if (_hour > WORLD_NIGHT_START || _hour < WORLD_NIGHT_END) {
        al_draw_rotated_bitmap(
                m_SkyBackground,
                512, 512,
                DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2,
                (float) engine_vars.counter / 1000,
                0
        );
    }

    DrawVerticalGradientRectangle(
            0, 0,
            DISPLAY_WIDTH, DISPLAY_HEIGHT,
            m_SkyTop, m_SkyBottom
    );

    m_Moon->Draw();
    m_Sun->Draw();

    // Clouds
#if 0
    DrawBitmap(
            m_CloudSprites[2],
            10 - game.camera_x / 4, 10 - game.camera_y,
            al_get_bitmap_width(m_CloudSprites[2]),
            al_get_bitmap_height(m_CloudSprites[2])
    );
#endif
    for (CloudObject &cloud : m_Clouds) cloud.Draw();
#else

    for (CloudObject &cloud : m_Clouds) cloud.Draw();

    al_draw_line(-1024, 400, 1024, 400, al_map_rgba(255, 255, 255, 30), 5);

#endif
}


