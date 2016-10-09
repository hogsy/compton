// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

EngineVars engine;

bool FileExists(const std::string path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

// Loaders

ALLEGRO_FONT *LoadFont(const std::string path, unsigned int size)
{
	std::string cpath = "./fonts/" + path + ".ttf";
	if(!FileExists(cpath)) cpath = "./fonts/" + path + ".otf";

	ALLEGRO_FONT *font = al_load_ttf_font(cpath.c_str(), size, 0);
	if(!font)
	{
		al_append_native_text_log(engine.log, "Failed to load font %s!\n", cpath.c_str());
		exit(-1);
	}

	return font;
}

ALLEGRO_BITMAP *LoadImage(const std::string path)
{
	std::string cpath = "./sprites/" + path + ".png";
	if(!FileExists(cpath)) cpath = "./sprites/" + path + ".bmp";

	ALLEGRO_BITMAP *bitmap = al_load_bitmap(cpath.c_str());
	if(!bitmap)
	{
		al_append_native_text_log(engine.log, "Failed to load bitmap %s!\n", cpath.c_str());
		exit(-1);
	}

	return bitmap;
}

// Draw Routines

void DrawBitmap(ALLEGRO_BITMAP *bitmap, float x, float y, int w, int h)
{
	if(!bitmap) return;
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

void DrawString(const ALLEGRO_FONT *font, int x, int y, ALLEGRO_COLOR colour, std::string message)
{
	if(!font) return;
	al_draw_text(font, colour, x, y, ALLEGRO_ALIGN_LEFT, message.c_str());
}

void DrawCenteredString(const ALLEGRO_FONT *font, int x, int y,	ALLEGRO_COLOR colour, std::string message)
{
	if(!font) return;
	al_draw_text(font, colour, x, y, ALLEGRO_ALIGN_CENTER, message.c_str());
}

void DrawFilledRectangle(PLVector2D position, float w, float h, ALLEGRO_COLOR colour)
{
	al_draw_filled_rectangle(position.x, position.y, w, h, colour);
}

void DrawVerticalGradientRectangle(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom)
{
	ALLEGRO_VERTEX v[4];
	memset(v, 0, sizeof(ALLEGRO_VERTEX) * 4);
	v[0].x = x; v[0].y = y; v[0].color = top;
	v[1].x = x + w; v[1].y = y; v[1].color = top;
	v[2].x = x; v[2].y = y + h; v[2].color = bottom;
	v[3].x = x + w; v[3].y = y + h; v[3].color = bottom;

	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

// Display

void InitializeDisplay()
{
	al_append_native_text_log(engine.log, "Initializing display...\n");

	engine.window_width		= WINDOW_WIDTH;
	engine.window_height 	= WINDOW_HEIGHT;

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
	engine.display = al_create_display(engine.window_width, engine.window_height);
	if(!engine.display)
	{
		al_show_native_message_box(
				NULL,
				"ERROR",
				"A serious fault occurred",
				"Failed to initialize display!",
				NULL,
				ALLEGRO_MESSAGEBOX_ERROR
		);
		exit(-1);
	}
	al_set_window_title(
			engine.display,
#ifdef DEBUG_BUILD
			"Virtual Critters Inc. [DEBUG]"
#else
			"Virtual Critters Inc."
#endif
	);

#if defined(DEBUG_BUILD)

#endif

	// Check to see how much we need to scale the buffer.
	engine.buffer = al_create_bitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	int sx = engine.window_width / DISPLAY_WIDTH;
	int sy = engine.window_height / DISPLAY_HEIGHT;
	int scale = std::min(sx, sy);
	engine.scalew = DISPLAY_WIDTH * scale;
	engine.scaleh = DISPLAY_HEIGHT * scale;
	engine.scalex = (engine.window_width - engine.scalew) / 2;
	engine.scaley = (engine.window_height - engine.scaleh) / 2;

	al_inhibit_screensaver(true);

	engine.redraw = true;
}

void DisplayFrame()
{
	if(!engine.redraw) return;

	// Buffer scaling.
	al_set_target_bitmap(engine.buffer);

	GameDisplayFrame();

	// Buffer scaling.
	al_set_target_backbuffer(engine.display);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap(
			engine.buffer,
			0, 0,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			engine.scalex, engine.scaley,
			engine.scalew, engine.scaleh,
			0
	);

	al_flip_display();

	engine.redraw = false;
}

void ShutdownDisplay()
{
	if(engine.display) al_destroy_display(engine.display);
}

// Events

void InitializeEvents()
{
	engine.timer = al_create_timer(1.0 / 60);
	if(!engine.timer)
	{
		al_show_native_message_box(
				NULL,
				"ERROR",
				"A serious fault occurred",
				"Failed to initialize timer!",
				NULL,
				ALLEGRO_MESSAGEBOX_ERROR
		);
		exit(-1);
	}

	engine.event_queue = al_create_event_queue();
	if(!engine.event_queue)
	{
		al_show_native_message_box(
				NULL,
				"ERROR",
				"A serious fault occurred",
				"Failed to initialize events!",
				NULL,
				ALLEGRO_MESSAGEBOX_ERROR
		);
		exit(-1);
	}

	al_register_event_source(engine.event_queue, al_get_display_event_source(engine.display));
	al_register_event_source(engine.event_queue, al_get_timer_event_source(engine.timer));

	al_install_mouse();
#if 1 // enable once we're happy with everything else.
	al_hide_mouse_cursor(engine.display);
#endif

	al_start_timer(engine.timer);
}

void EventsFrame()
{
	ALLEGRO_EVENT event;
	al_wait_for_event(engine.event_queue, &event);

	al_get_mouse_state(&engine.mouse_state);

	switch(event.type)
	{
		default:break;

		case ALLEGRO_EVENT_TIMER:
			engine.counter++;
			GameTimerFrame();
			engine.redraw = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			engine.running = false;
			break;
	}

	if(!al_is_event_queue_empty(engine.event_queue))
		engine.redraw = false;
}

void ShutdownEvents()
{
	if(engine.event_queue) al_destroy_event_queue(engine.event_queue);
	if(engine.timer) al_destroy_timer(engine.timer);
}

// Main

int main()
{
	plClearLog(VC_LOG);

	plWriteLog(VC_LOG, "Virtual Critters " VC_VERSION " (" __DATE__ ")\n\n");

	PLuint32 version = al_get_allegro_version();
	PLuint32 major = version >> 24;
	PLuint32 minor = (version >> 16) & 255;
	plWriteLog(VC_LOG, "Initializing Allegro %i.%i...\n", major, minor);
	if(!al_init())
	{
		plMessageBox(VC_TITLE, "Failed to initialize Allegro library!\n");
		return -1;
	}
	else if(!al_install_mouse())
	{
		plMessageBox(VC_TITLE, "Failed to install mouse through Allegro!\n");
		return -1;
	}

	al_init_native_dialog_addon();
	al_init_primitives_addon();
	if(!al_init_image_addon())
	{
		plMessageBox(VC_TITLE, "Failed to initialize Allegro image library!\n");
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_change_directory(al_path_cstr(path, '/'));
	al_destroy_path(path);

	// Doing this to ensure that rand is truly random
	// otherwise, for example, clouds will always spawn
	// in the same places every time.
	srand((unsigned)time(NULL));

	//engine.log = al_open_native_text_log("Virtual Critters Console", 0);

	memset(&engine, 0, sizeof(EngineVars));

	engine.running = true;

	InitializeDisplay();
	InitializeEvents();
	InitializeGame();

	while(engine.running)
	{
		EventsFrame();
		DisplayFrame();
	}

	ShutdownGame();
	ShutdownDisplay();
	ShutdownEvents();

	return 0;
}