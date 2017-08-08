// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../../shared.h"

#include "../game.h"
#include "object_world.h"

World *game_worldmanager = nullptr;

#define WORLD_NIGHT_START   18  // Starting hour for night to roll in.
#define WORLD_NIGHT_END     6   // Final hour that night starts rolling out.

/*	Clouds	*/

#define DAMP    ((w * h) / ((float)plGenerateUniformRandom(0.05f) + 5)) / 100;
#define JIGGLE  0.05f

class RainObject : public Sprite {
public:
    RainObject(PLVector2D pos) : Sprite(World::GetInstance()->cloud_droplet) {
        position_ = pos;
    }

    void Simulate() {
        if (!IsVisible()) {
            delete this;
        }
        position_.x += 0.5f;
        position_.y += (World::GetInstance()->GetWindSpeed() / 10.0f);
        angle = -(World::GetInstance()->GetWindSpeed() / 100.0f);
    }

protected:
private:
};

class CloudObject : public Sprite {
public:
    explicit CloudObject(ALLEGRO_BITMAP *sprite) : Sprite(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        position_.Set((std::rand() % DISPLAY_WIDTH) + 1, (std::rand() % (DISPLAY_HEIGHT / 2)) + 1);
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    CloudObject(ALLEGRO_BITMAP *sprite, bool direction) : Sprite(sprite) {
        w = al_get_bitmap_width(sprite);
        h = al_get_bitmap_height(sprite);

        position_.Set((direction ? -(float) w : DISPLAY_WIDTH), std::rand() % (DISPLAY_HEIGHT / 2));
        _damping = DAMP;
        _jiggle = (float) plGenerateUniformRandom(JIGGLE);
    }

    void Move(float speed) {
        position_.x += speed / _damping;
        static double s = std::rand() % 100;
        angle = std::cos((float) s++ / 1000) * 0.05f;
    }

    void Draw() override {
        PLVector2D oldpos = position_;
        position_.y = (std::sin((float) engine_vars.counter / (120 / _jiggle)) * 5 + 5) + position_.y;

        position_.x -= game.camera_x;
        position_.y -= game.camera_y;

        Sprite::Draw();
        position_ = oldpos;
    }

    int w, h;

protected:
private:
    float _damping, _jiggle;
};

#include "object_moon.h"

class Sun : public Sprite {
public:
    Sun() : Sprite(engine::LoadImage("environment/objects/sun")) {
        origin_.x = al_get_bitmap_width(GetBitmap()) / 2;
        origin_.y = -420;
        position_.Set(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT);

        angle = 0;
    }

    void Simulate() {
        angle = World::GetInstance()->GetTotalSeconds() * 60 / 360;
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

    static void LoadNightBackground(const char *path) {
        std::printf("FOUND BACKGROUND LEL!\n");

        std::string properpath = path + 6;

    }

    void AddNightBackground(ALLEGRO_BITMAP *bitmap) {
        night.push_back(bitmap);
        if (night[night.size()] != nullptr) {}
    }

    void Draw() {
        // Bleh, but only do this if a background is set...
        if (current_background == nullptr) return;

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

#define DEFAULT_WIND_SPEED 10.5f

#define CLOUD_BITMAPS 3

#include "names.h"
#include "object_creature.h"

World::World() :
        wind_speed_(DEFAULT_WIND_SPEED),
        time_(0), year_(0), m_Month(0), _day(0), _hour(0), _minute(0), _second(0),
        width_(4000), height_(4000),
        cloud_density_(8),
        temperature_(20) {
    env_background = new EnvironmentBackground();

    name_ = planet_names[rand() % plArrayElements(planet_names)];

    cloud_sprites_.reserve(10);
    for (int i = 0; i < CLOUD_BITMAPS; i++) {
        cloud_sprites_[i] = engine::LoadImage(std::string("clouds/" + std::to_string(i)).c_str());
    }

    sky_top_            = sky_colourcycle[0].top;
    sky_toptarget_      = sky_colourcycle[1].top;
    sky_bottom_         = sky_colourcycle[0].bottom;
    sky_bottomtarget_   = al_map_rgb(0, 0, 0); //sky_colourcycle[1].bottom;
    sky_background_     = engine::LoadImage("sprites");

    cloud_droplet = engine::LoadImage("environment/objects/rain");

    // Make initial set of clouds.
    for (int i = 0; i < cloud_density_; i++) {
        ALLEGRO_BITMAP *sprite = cloud_sprites_[rand() % CLOUD_BITMAPS];
        m_Clouds.emplace(m_Clouds.end(), CloudObject(sprite));
    }

    creature_ = new Creature();
}

World::~World() {

}

const char *World::GetDayString() {
    return sky_days[_day];
}

const char *World::GetMonthString() {
    return months[m_Month].name;
}

void World::Simulate() {
    if(game.menu_state != GAME_MENU_DEFAULT) {
        return;
    }

    // CloudObject generation...
#if 0
    if (m_Clouds.size() < cloud_density_) {
        ALLEGRO_BITMAP *sprite = cloud_sprites_[rand() % CLOUD_BITMAPS];
        m_Clouds.emplace(
                m_Clouds.end(),
                CloudObject(
                        sprite,
                        (bool) wind_speed_
                )
        );
    }

    for (unsigned int i = 0; i < m_Clouds.size(); ++i) {
        m_Clouds[i].Move(wind_speed_);
        if (!m_Clouds[i].IsVisible())
            m_Clouds.erase(m_Clouds.begin() + i);
    }
#endif

    static double difference = 1;

    time_++;
    _second++;
    if (_second > 60) {
        _minute += 1;
        _second = 0;
    }
    if (_minute > 60) {
        _hour += 1;
        _minute = 0;
        _second = 0;
        for (auto &i : sky_colourcycle) {
            if (_hour >= i.hour) {
                sky_toptarget_ = i.top;
                sky_bottomtarget_ = i.bottom;

                difference = i.hour / _hour;
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
#define INTERP (float)((time_ / 60) / 4)
    // BOTTOM
    sky_bottom_.r = plCosineInterpolate(
            sky_bottom_.r, sky_bottomtarget_.r,
            INTERP
    );
    sky_bottom_.g = plCosineInterpolate(
            sky_bottom_.g, sky_bottomtarget_.g,
            INTERP
    );
    sky_bottom_.b = plCosineInterpolate(
            sky_bottom_.b, sky_bottomtarget_.b,
            INTERP
    );
    // TOP
    sky_top_.r = plCosineInterpolate(
            sky_top_.r, sky_toptarget_.r,
            INTERP
    );
    sky_top_.g = plCosineInterpolate(
            sky_top_.g, sky_toptarget_.g,
            INTERP
    );
    sky_top_.b = plCosineInterpolate(
            sky_top_.b, sky_toptarget_.b,
            INTERP
    );

    creature_->Simulate();

//    moon_->Simulate();
 //   sun_->Simulate();
}

void World::Draw() {
    if(game.menu_state != GAME_MENU_DEFAULT) {
        return;
    }

    al_draw_bitmap_region(sky_background_, 0, 0, 64, 64, 0, 0, 0);

    creature_->Draw();
}


