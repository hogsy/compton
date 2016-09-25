// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

/*	Game logic and other crap goes here!	*/

enum GameState {
	GAME_STATE_MENU,
	GAME_STATE_PAUSED,
} GameState;

typedef struct GameVars {
	unsigned int state;
	unsigned int menu_state;

	double counter;

	// Simulation
	float wind_speed;

	ALLEGRO_FONT *font_title;
	ALLEGRO_FONT *font_small, *font_gothic_medium;
	ALLEGRO_FONT *font_chunk;
} GameVars;

GameVars game;

// Object

class SpriteObject
{
public:
	SpriteObject() : _sprite(nullptr), origin(0, 0), position(0, 0), angle(0), _w(0), _h(0) {}
	SpriteObject(ALLEGRO_BITMAP *sprite) : _sprite(sprite), origin(0, 0), position(0, 0), angle(0)
	{
		_w = al_get_bitmap_width(sprite); _h = al_get_bitmap_height(sprite);
		origin.Set(_w / 2, _h / 2);
	}
	SpriteObject(ALLEGRO_BITMAP *sprite, float x, float y) : _sprite(sprite), origin(0, 0), position(x, y), angle(0)
	{
		_w = al_get_bitmap_width(sprite); _h = al_get_bitmap_height(sprite);
		origin.Set(_w / 2, _h / 2);
	}

	ALLEGRO_BITMAP *GetSprite() { return _sprite; }
	void SetSprite(ALLEGRO_BITMAP *sprite)
	{
		_sprite = sprite;
		_w = al_get_bitmap_width(sprite); _h = al_get_bitmap_height(sprite);
	}

	bool InsideBounds()
	{
		if(position.x < -(float)_w || position.x > DISPLAY_WIDTH + _w)
			return false;

		return true;
	}

	virtual void Draw()
	{
		if(!InsideBounds()) return;
		al_draw_scaled_rotated_bitmap(
				_sprite,
				origin.x, origin.y,
				position.x, position.y,
				1, 1,
				angle,
				0
		);
	}

	Vector2D position;
	Vector2D origin;
	float angle;

protected:
private:
	ALLEGRO_BITMAP *_sprite;

	int _w, _h;
};

// Glorious Clouds

#define DAMP 	((w * h) / (GenerateRandom(0.05f) + 5)) / 100;
#define JIGGLE	0.05f

class Cloud : public SpriteObject
{
public:
	Cloud(ALLEGRO_BITMAP *sprite) : SpriteObject(sprite)
	{
		w = al_get_bitmap_width(sprite);
		h = al_get_bitmap_height(sprite);

		position.Set(rand()%DISPLAY_WIDTH, rand()%(DISPLAY_HEIGHT/2));
		_damping = DAMP;
		_jiggle = GenerateRandom(JIGGLE);
	}

	Cloud(ALLEGRO_BITMAP *sprite, bool direction) : SpriteObject(sprite)
	{
		w = al_get_bitmap_width(sprite);
		h = al_get_bitmap_height(sprite);

		position.Set((direction ? -(float)w : DISPLAY_WIDTH ), rand()%(DISPLAY_HEIGHT / 2));
		_damping = DAMP;
		_jiggle = GenerateRandom(JIGGLE);
	}

	void Move(float speed)
	{
		position.x += speed / _damping;
		static double s = rand()%100;
		angle = std::cos((float)s++ / 1000) * 0.05f;
	}

	virtual void Draw()
	{
		Vector2D oldpos = position;
		position.y = (std::sin((float)game.counter / (120 / _jiggle)) * 5 + 5) + position.y;
		SpriteObject::Draw();
		position = oldpos;
	}

	int w, h;

protected:
private:
	float _damping, _jiggle;
};

#define WIND_SPEED 10.5f

class SkyManager
{
public:
	SkyManager() : _wind_speed(WIND_SPEED)
	{
		earth_back = LoadImage("clouds/earth");

		_cloud_sprites.reserve(10);
		for(int i = 0; i < 10; i++)
			_cloud_sprites[i] = LoadImage("clouds/" + std::to_string(i));

		// Make initial set of clouds.
		for(int i = 0; i < 8; i++)
		{
			ALLEGRO_BITMAP *sprite = _cloud_sprites[rand()%10];
			_clouds.emplace(_clouds.end(), Cloud(sprite));
		}
	}

	~SkyManager()
	{
		for(int i = 0; i < _cloud_sprites.size(); i++)
			al_destroy_bitmap(_cloud_sprites[i]);
		_cloud_sprites.clear();

		if(earth_back) al_destroy_bitmap(earth_back);
	}

	void Simulate()
	{
		if(_clouds.size() < 8)
		{
			ALLEGRO_BITMAP *sprite = _cloud_sprites[rand()%10];
			_clouds.emplace(
					_clouds.end(),
					Cloud(
							sprite,
							(bool)_wind_speed
					)
			);
		}

#if 0
		static int delay = 0;
		delay++; if(delay < 5) return;
		delay = 0;
#endif

		for(unsigned int i = 0; i < _clouds.size(); ++i)
		{
			_clouds[i].Move(_wind_speed);
			if(!_clouds[i].InsideBounds())
				_clouds.erase(_clouds.begin() + i);
		}
	}

	void Draw()
	{
		DrawVerticalGradientRectangle(
				0, 0,
				DISPLAY_WIDTH, DISPLAY_HEIGHT,
				al_map_rgb(0, 140, 186), al_map_rgb(76, 187, 224)
		);

		if(game.state == GAME_STATE_MENU) {
			al_draw_rotated_bitmap(
					earth_back,
					al_get_bitmap_width(earth_back) / 2,
					al_get_bitmap_height(earth_back) / 2,
					(DISPLAY_WIDTH / 2),
					850,
					(float) (game.counter / 1000),
					0
			);
		}

		DrawBitmap(
				_cloud_sprites[2],
				10, 10,
				al_get_bitmap_width(_cloud_sprites[2]),
				al_get_bitmap_height(_cloud_sprites[2])
		);
		for(Cloud &cloud : _clouds)	cloud.Draw();
	}

protected:
private:
	float _wind_speed;

	ALLEGRO_BITMAP *earth_back;

	std::vector<Cloud> _clouds;
	std::vector<ALLEGRO_BITMAP*> _cloud_sprites;
};

SkyManager *game_skymanager;

// Menu (includes HUD etc.)

enum GameMenuState {
	GAME_MENU_START,		// "Press start to begin the game!"
	GAME_MENU_MAIN,			// Main Menu
		GAME_MENU_NEW,		// New Game
		GAME_MENU_LOAD,		// Load Game
		GAME_MENU_OPTIONS,	// Options
		GAME_MENU_QUIT,		// Quit

	GAME_MENU_GAME			// in-game?
} GameMenuState;

void DrawGameMenu()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	game_skymanager->Draw();

	switch(game.menu_state)
	{
		default:break;

		case GAME_MENU_START:
		{
			const int grad_height = 128;
			const int grad_alpha = 62;
			DrawVerticalGradientRectangle(
					0, 0,
					DISPLAY_WIDTH, grad_height,
					al_map_rgba(0, 0, 0, grad_alpha),
					al_map_rgba(0, 0, 0, 0)
			);
			DrawVerticalGradientRectangle(
					0, DISPLAY_HEIGHT - grad_height,
					DISPLAY_WIDTH, grad_height,
					al_map_rgba(0, 0, 0, 0),
					al_map_rgba(0, 0, 0, grad_alpha)
			);
			DrawVerticalGradientRectangle(
					0, 153,
					DISPLAY_WIDTH, 80,
					al_map_rgba(0, 0, 0, 150),
					al_map_rgba(0, 0, 0, 150)
			);
			DrawCenteredString(
					game.font_title,
					DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4,
					/*230, 80, 20*/ al_map_rgb(255, 255, 255),
					"Virtual Critters Inc."
			);
#if 0
			DrawCenteredString(
					game.font_small,
					DISPLAY_WIDTH / 2,
					DISPLAY_HEIGHT - 42,
					al_map_rgb(255, 255, 255),
					"Virtual Critters Inc., Copyright (C) 2016 Mark Elsworth Sowden"
			);
#endif
			DrawCenteredString(
					game.font_gothic_medium,
					DISPLAY_WIDTH / 2, 250,
					al_map_rgb(
							(byte)(std::cos(game.counter / 30) * 127 + 127),
							(byte)(std::cos(game.counter / 30) * 127 + 127),
							(byte)(std::cos(game.counter / 30) * 127 + 127)
					),
					"PRESS ANY KEY TO START"
			);
		}
		break;

		case GAME_MENU_MAIN:
		{}
		break;

		case GAME_MENU_NEW:
		{}
		break;

		case GAME_MENU_LOAD:
		{}
		break;

		case GAME_MENU_OPTIONS:
		{}
		break;

		case GAME_MENU_QUIT:
		{}
		break;

		case GAME_MENU_GAME:
		{}
		break;
	}
}

// Everything Else

void InitializeGame()
{
	memset(&game, 0, sizeof(GameVars));

	game.state = GAME_STATE_MENU;
	game.menu_state = GAME_MENU_START;

	//LoadFont("pacifico/Pacifico-Regular", 24);
	//LoadFont("chunk/Chunk", 24);
	game.font_title = LoadFont("pacifico/Pacifico-Bold", 80);
	game.font_small = LoadFont("league_gothic/LeagueGothic-Regular", 21);
	game.font_gothic_medium = LoadFont("league_gothic/LeagueGothic-Regular", 32);
	game.font_chunk = LoadFont("chunk/Chunk", 24);

	game_skymanager = new SkyManager();
}

void GameDisplayFrame()
{
	DrawGameMenu();
}

void GameTimerFrame()
{
	game.counter++;

	switch(game.state)
	{
		case GAME_STATE_MENU:
			game_skymanager->Simulate();
			break;

		case GAME_STATE_PAUSED:break;

		default:break;
	}
}

void ShutdownGame()
{}

