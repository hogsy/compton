// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../shared.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

EngineVars engine_vars;

// Loaders

namespace engine {
    std::unordered_map<std::string, ALLEGRO_BITMAP *> bitmaps;

    ALLEGRO_FONT *LoadFont(const char *path, unsigned int size) {
        char path1[PL_SYSTEM_MAX_PATH] = { 0 };
        std::sprintf(path1, "./fonts/%s.ttf", path);
        if (!plFileExists(path1)) {
            std::sprintf(path1, "./fonts/%s.otf", path);
        }

        ALLEGRO_FONT *font = al_load_ttf_font(path1, size, 0);
        if (font == nullptr) {
            plWriteLog(VC_LOG, "Failed to load font %s!\n", path1);
            plMessageBox("Failed to load font %s!\n", path1);
            exit(-1);
        }

        return font;
    }

    ALLEGRO_BITMAP *LoadImage(const char *path) {
        char path1[PL_SYSTEM_MAX_PATH] = { 0 };
        std::sprintf(path1, "./sprites/%s.png", path);
        if (!plFileExists(path1)) {
            std::sprintf(path1, "./sprites/%s.bmp", path);
        }

        auto i = bitmaps.find(path1);
        if(i != bitmaps.end()) {
            return i->second;
        }

        ALLEGRO_BITMAP *bitmap = al_load_bitmap(path1);
        if (bitmap == nullptr) {
            plWriteLog(VC_LOG, "Failed to load bitmap %s!\n", path1);
            plMessageBox("Failed to load bitmap %s!\n", path1);
            exit(-1);
        }
        bitmaps.emplace(path1, bitmap);

        return bitmap;
    }
}

void DisplayMessageBox(const std::string &title, const std::string &message, bool error) {
    al_show_native_message_box(
            nullptr,
            VC_TITLE,
            title.c_str(),
            message.c_str(),
            nullptr,
            error ? ALLEGRO_MESSAGEBOX_ERROR : ALLEGRO_MESSAGEBOX_WARN
    );
}

// Draw Routines

void DrawBitmap(ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h) {
    if (bitmap == nullptr) return;
    al_draw_scaled_bitmap(
            bitmap,
            0, 0,
            al_get_bitmap_width(bitmap),
            al_get_bitmap_height(bitmap),
            x, y,
            w, h,
            0
    );
}

void DrawString(const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message) {
    if (font == nullptr) {
        return;
    }
    al_draw_text(font, colour, x, y, ALLEGRO_ALIGN_LEFT, message);
}

void DrawCenteredString(const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, const char *message) {
    if (font == nullptr) {
        return;
    }
    al_draw_text(font, colour, x, y, ALLEGRO_ALIGN_CENTER, message);
}

void DrawFilledRectangle(PLVector2D position, float w, float h, ALLEGRO_COLOR colour) {
    al_draw_filled_rectangle(position.x, position.y, w, h, colour);
}

void DrawVerticalGradientRectangle(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom) {
    ALLEGRO_VERTEX v[4];
    memset(v, 0, sizeof(ALLEGRO_VERTEX) * 4);
    v[0].x = x;     v[0].y = y;     v[0].color = top;
    v[1].x = x + w; v[1].y = y;     v[1].color = top;
    v[2].x = x;     v[2].y = y + h; v[2].color = bottom;
    v[3].x = x + w; v[3].y = y + h; v[3].color = bottom;

    al_draw_prim(v, nullptr, nullptr, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

// Display

void InitializeDisplay() {
    plWriteLog(VC_LOG, "Initializing display...\n");

    engine_vars.window_width = WINDOW_WIDTH;
    engine_vars.window_height = WINDOW_HEIGHT;

#if 1
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    engine_vars.display = al_create_display(engine_vars.window_width, engine_vars.window_height);
    if (engine_vars.display == nullptr) {
        al_show_native_message_box(
                nullptr,
                "ERROR",
                "A serious fault occurred",
                "Failed to initialize display!",
                nullptr,
                ALLEGRO_MESSAGEBOX_ERROR
        );
        exit(-1);
    }
    al_set_window_title(
            engine_vars.display,
#ifdef DEBUG_BUILD
            "Virtual Critters Inc. [DEBUG]"
#else
            "Virtual Critters Inc."
#endif
    );
#else // platform lib window creation
    PLWindow *window = plCreateWindow(
#ifdef DEBUG_BUILD
            "Virtual Critters Inc. [DEBUG]",
#else
            "Virtual Critters Inc.",
#endif
            0, 0, engine_vars.window_width, engine_vars.window_height
    );
    if(!window) {
        plMessageBox("ERROR", "Failed to initialize display!\n");
        exit(-1);
    }
#endif

    // Check to see how much we need to scale the buffer.
    int flags = al_get_new_bitmap_flags();
    //al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);
    engine_vars.buffer = al_create_bitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_set_new_bitmap_flags(flags);
    int sx = engine_vars.window_width / DISPLAY_WIDTH;
    int sy = engine_vars.window_height / DISPLAY_HEIGHT;
    int scale = std::min(sx, sy);

    engine_vars.scalew = DISPLAY_WIDTH * scale;
    engine_vars.scaleh = DISPLAY_HEIGHT * scale;
    engine_vars.scalex = (engine_vars.window_width - engine_vars.scalew) / 2;
    engine_vars.scaley = (engine_vars.window_height - engine_vars.scaleh) / 2;

    al_inhibit_screensaver(true);

    al_set_clipping_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    engine_vars.redraw = true;
}

void DisplayFrame() {
    if (!engine_vars.redraw) {
        return;
    }

    // Buffer scaling.
    al_set_target_bitmap(engine_vars.buffer);

    GameDisplayFrame();

    // Buffer scaling.
    al_set_target_backbuffer(engine_vars.display);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(
            engine_vars.buffer,
            0, 0,
            DISPLAY_WIDTH, DISPLAY_HEIGHT,
#if 0
            engine_vars.scalex, engine_vars.scaley,
            engine_vars.scalew, engine_vars.scaleh,
#else
            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
#endif
            0
    );

    al_flip_display();

    engine_vars.redraw = false;
}

void ShutdownDisplay() {
    if (engine_vars.display != nullptr) {
        al_destroy_display(engine_vars.display);
    }
}

// Events

void InitializeEvents() {
    std::printf("Initializing input...\n");

    if ((engine_vars.timer = al_create_timer(1.0 / 60)) == nullptr) {
        DisplayMessageBox("ERROR", "Failed to initialize timer!", true);
        exit(-1);
    }

    if ((engine_vars.event_queue = al_create_event_queue()) == nullptr) {
        DisplayMessageBox("ERROR", "Failed to initialize events!", true);
        exit(-1);
    }

    al_install_mouse();
    al_install_keyboard();

#if 0 // enable once we're happy with everything else.
    al_hide_mouse_cursor(engine_vars.display);
#endif

    al_register_event_source(engine_vars.event_queue, al_get_display_event_source(engine_vars.display));
    al_register_event_source(engine_vars.event_queue, al_get_timer_event_source(engine_vars.timer));
    al_register_event_source(engine_vars.event_queue, al_get_keyboard_event_source());

    al_start_timer(engine_vars.timer);
}

void EventsFrame() {
    ALLEGRO_EVENT event{};
    al_wait_for_event(engine_vars.event_queue, &event);

    al_get_mouse_state(&engine_vars.mouse_state);
    al_get_keyboard_state(&engine_vars.keyboard_state);

    engine_vars.mouse_status[BUTTON_LMOUSE] = (engine_vars.mouse_state.buttons & 1) != 0;
    engine_vars.mouse_status[BUTTON_RMOUSE] = (engine_vars.mouse_state.buttons & 2) != 0;
    engine_vars.mouse_status[BUTTON_MMOUSE] = (engine_vars.mouse_state.buttons & 3) != 0;

    switch (event.type) {
        default: break;

        case ALLEGRO_EVENT_TIMER:
            engine_vars.counter++;
            GameTimerFrame();
            engine_vars.redraw = true;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            engine_vars.running = false;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES: {
            MouseEvent();
            break;
        }

        case ALLEGRO_EVENT_KEY_DOWN: {
            engine_vars.key_status[event.keyboard.keycode] = true;

            KeyboardEvent(event.keyboard.keycode, false);
            break;
        }
        case ALLEGRO_EVENT_KEY_UP: {
            engine_vars.key_status[event.keyboard.keycode] = false;

            KeyboardEvent(event.keyboard.keycode, true);
            break;
        }
    }

    if (!al_is_event_queue_empty(engine_vars.event_queue)) {
        engine_vars.redraw = false;
    }
}

void ShutdownEvents() {
    if (engine_vars.event_queue != nullptr) al_destroy_event_queue(engine_vars.event_queue);
    if (engine_vars.timer != nullptr)       al_destroy_timer(engine_vars.timer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shutdown() {
    ShutdownGame();
    ShutdownDisplay();
    ShutdownEvents();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main

int main(int argc, char **argv) {

    std::cout << "Virtual Critters " << VC_VERSION << " (" << __DATE__ << ")\n\n";

    memset(&engine_vars, 0, sizeof(EngineVars));

    uint32_t version = al_get_allegro_version();
    uint32_t major = version >> 24;
    uint32_t minor = (version >> 16) & 255;
    std::cout << "Initializing Allegro " << major << "." << minor << "\n";
    if (!al_init()) {
        DisplayMessageBox("ERROR", "Failed to initialize Allegro library!", true);
        return -1;
    }

    if (!al_install_mouse()) {
        DisplayMessageBox("ERROR", "Failed to install mouse through Allegro!", true);
        return -1;
    }

    if(!al_install_audio()) {
        DisplayMessageBox("ERROR", "Failed to install audio through Allegro!", true);
        return -1;
    }

    if(!al_init_acodec_addon()) {
        DisplayMessageBox("ERROR", "Failed to install audio codecs through Allegro!", true);
        return -1;
    }
    al_init_native_dialog_addon();
    al_init_primitives_addon();
    if (!al_init_image_addon()) {
        DisplayMessageBox("ERROR", "Failed to initialize Allegro image library!", true);
        return -1;
    }
    al_init_font_addon();
    al_init_ttf_addon();

    al_reserve_samples(512);

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_change_directory(al_path_cstr(path, '/'));
    al_destroy_path(path);

    // Doing this to ensure that rand is truly random
    // otherwise, for example, clouds will always spawn
    // in the same places every time.
    srand((unsigned) time(nullptr));

    engine_vars.running = true;

    InitializeDisplay();
    InitializeEvents();
    InitializeGame();

    while (engine_vars.running) {
        EventsFrame();
        DisplayFrame();
    }

    Shutdown();

    return 0;
}