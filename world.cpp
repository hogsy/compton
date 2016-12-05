// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

#include "game.h"
#include "world.h"
#include "object_moon.h"

WorldManager *game_skymanager = nullptr;

#define WORLD_NIGHT_START   18  // Starting hour for night to roll in.
#define WORLD_NIGHT_END     6   // Final hour that night starts rolling out.

/*	Clouds	*/

#define DAMP    ((w * h) / ((float)plGenerateUniformRandom(0.05f) + 5)) / 100;
#define JIGGLE    0.05f

class RainDrop : public SpriteObject {
public:
    RainDrop(PLVector2D pos) : SpriteObject(game_skymanager->cloud_droplet) {
        position = pos;
    }

    void Simulate() {
        if (!InsideBounds()) {
            delete this;
        }
        position.x += 0.5f;
        position.y += (game_skymanager->GetWindSpeed() / 10.0f);
        angle = -(game_skymanager->GetWindSpeed() / 100.0f);
    }

protected:
private:
};

class Cloud : public SpriteObject {
public:
    Cloud(ALLEGRO_BITMAP *sprite) : SpriteObject(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        position.Set(rand() % DISPLAY_WIDTH, rand() % (DISPLAY_HEIGHT / 2));
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    Cloud(ALLEGRO_BITMAP *sprite, bool direction) : SpriteObject(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        position.Set((direction ? -(float) w : DISPLAY_WIDTH), rand() % (DISPLAY_HEIGHT / 2));
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    void Move(float speed) {
        position.x += speed / _damping;
        static double s = rand() % 100;
        angle = std::cos((float) s++ / 1000) * 0.05f;
    }

    virtual void Draw() {
        PLVector2D oldpos = position;
        position.y = (std::sin((float) engine.counter / (120 / _jiggle)) * 5 + 5) + position.y;
        SpriteObject::Draw();
        position = oldpos;
    }

    int w, h;

protected:
private:
    float _damping, _jiggle;
};

#include "object_moon.h"

class Sun : public SpriteObject {
public:
    Sun() : SpriteObject(LoadImage("environment/objects/sun")) {
        origin.x = al_get_bitmap_width(GetSprite()) / 2;
        origin.y = -420;
        position.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

        angle = 0;
    }

    void Simulate() {
        angle = game_skymanager->GetTotalSeconds() * 60 / 360;
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

        al_draw_tinted_scaled_rotated_bitmap(
                current_background,
                al_map_rgb(255, 255, 255),
                256, 256,
                DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2,
                1024, 1024,
                (float) engine.counter / 100,
                0
        );
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

std::string sky_days[] = {
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

    std::string name;
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

WorldManager::WorldManager() :
        _wind_speed(WIND_SPEED),
        _time(0), _year(0), _month(0), _day(0), _hour(0), _minute(0), _second(0),
        _cloud_density(8),
        _temperature(20) {
    env_background = new EnvironmentBackground();

    _cloud_sprites.reserve(10);
    for (int i = 0; i < 10; i++)
        _cloud_sprites[i] = LoadImage("clouds/" + std::to_string(i));

    _sky_top =  //sky_colourcycle[0].top;
    _sky_target_top = //sky_colourcycle[1].top;
    _sky_bottom = //sky_colourcycle[0].bottom;
    _sky_target_bottom = al_map_rgb(0, 0, 0); //sky_colourcycle[1].bottom;
    _sky_background = LoadImage("environment/backgrounds/00night");

    cloud_droplet = LoadImage("environment/objects/rain");

    // Make initial set of clouds.
    for (int i = 0; i < _cloud_density; i++) {
        ALLEGRO_BITMAP *sprite = _cloud_sprites[rand() % 10];
        _clouds.emplace(_clouds.end(), Cloud(sprite));
    }

    _moon = new Moon();
    _sun = new Sun();
}

WorldManager::~WorldManager() {
    for (int i = 0; i < _cloud_sprites.size(); i++)
        al_destroy_bitmap(_cloud_sprites[i]);
    _cloud_sprites.clear();

    delete _moon;
}

std::string WorldManager::GetDayString() {
    return sky_days[_day];
}

void WorldManager::Simulate() {
    // Cloud generation...
    if (_clouds.size() < _cloud_density) {
        ALLEGRO_BITMAP *sprite = _cloud_sprites[rand() % 10];
        _clouds.emplace(
                _clouds.end(),
                Cloud(
                        sprite,
                        (bool) _wind_speed
                )
        );
    }

    for (unsigned int i = 0; i < _clouds.size(); ++i) {
        _clouds[i].Move(_wind_speed);
        if (!_clouds[i].InsideBounds())
            _clouds.erase(_clouds.begin() + i);
    }

    static double difference = 1;

    _time++;
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
                _sky_target_top = sky_colourcycle[i].top;
                _sky_target_bottom = sky_colourcycle[i].bottom;

                difference = sky_colourcycle[i].hour / _hour;
            }
        }
    }
    if (_hour > 24) {
        _day += 1;
        _hour = 0;
        _minute = 0;
        _second = 0;
    }
    if (_day > 6) {
        _day = 0;
        _hour = 0;
        _minute = 0;
        _second = 0;
    }

    // Sky gradient
    //int difference =
#define INTERP (float)((_time / 60) / 4)
    // BOTTOM
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
    // TOP
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

    _moon->Simulate();
    _sun->Simulate();
}

void WorldManager::Draw() {
    if (_hour > WORLD_NIGHT_START || _hour < WORLD_NIGHT_END) {
        al_draw_rotated_bitmap(
                _sky_background,
                512, 512,
                DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2,
                (float) engine.counter / 1000,
                0
        );
    }

    DrawVerticalGradientRectangle(
            0, 0,
            DISPLAY_WIDTH, DISPLAY_HEIGHT,
            _sky_top, _sky_bottom
    );

    _moon->Draw();
    _sun->Draw();

    // Clouds
    DrawBitmap(
            _cloud_sprites[2],
            10, 10,
            al_get_bitmap_width(_cloud_sprites[2]),
            al_get_bitmap_height(_cloud_sprites[2])
    );
    for (Cloud &cloud : _clouds) cloud.Draw();
}
