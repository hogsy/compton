// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#include "shared.h"

#include "game.h"

#include "alife.h"
#include "sky_manager.h"

/*	Game logic and other crap goes here!	*/

enum GameState {
	GAME_STATE_DEFAULT,
	GAME_STATE_MENU,
	GAME_STATE_PAUSED,
} GameState;

GameVars game;

// Object

SpriteObject::SpriteObject() :
		_sprite(nullptr), origin(0, 0), position(0, 0), angle(0), _w(0), _h(0),
		currentframe(0), framecount(0), framedelay(0) {}
SpriteObject::SpriteObject(ALLEGRO_BITMAP *sprite) :
		_sprite(sprite), origin(0, 0), position(0, 0), angle(0),
		currentframe(0), framecount(0), framedelay(0)
{
	framew = _w = al_get_bitmap_width(sprite); frameh = _h = al_get_bitmap_height(sprite);
	origin.Set(_w / 2, _h / 2);
}
SpriteObject::SpriteObject(ALLEGRO_BITMAP *sprite, float x, float y) :
		_sprite(sprite), origin(0, 0), position(x, y), angle(0),
		currentframe(0), framecount(0), framedelay(0)
{
	framew = _w = al_get_bitmap_width(sprite); frameh =  _h = al_get_bitmap_height(sprite);
	origin.Set(_w / 2, _h / 2);
}

SpriteObject::~SpriteObject()
{
	if(_sprite) al_destroy_bitmap(_sprite);
}

void SpriteObject::SetSprite(ALLEGRO_BITMAP *sprite)
{
	_sprite = sprite;
	if(_sprite) { _w = al_get_bitmap_width(sprite); _h = al_get_bitmap_height(sprite); }
}

void SpriteObject::Draw()
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

bool SpriteObject::InsideBounds()
{
	if(position.x < -(float)_w || position.x > DISPLAY_WIDTH + _w)
		return false;

	return true;
}

// Hand

class Hand : public SpriteObject
{
public:
	Hand() : SpriteObject(LoadImage("hand_sheet"))
	{
		// todo, set this in position of cursor on creation.
	}

protected:
private:
};

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
			al_draw_rotated_bitmap(
					game.menu_earth,
					al_get_bitmap_width(game.menu_earth) / 2,
					al_get_bitmap_height(game.menu_earth) / 2,
					(DISPLAY_WIDTH / 2),
					850,
					(float) (engine.counter / 1000),
					0
			);

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
					al_map_rgba(
							0, //(byte)(std::cos(game.counter / 30) * 127 + 127),
							0, //(byte)(std::cos(game.counter / 30) * 127 + 127),
							0, //(byte)(std::cos(game.counter / 30) * 127 + 127),
							(byte)(std::cos(engine.counter / 30) * 50 + 127)
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

	game.state = GAME_STATE_DEFAULT;
	game.menu_state = GAME_MENU_GAME;

	//LoadFont("pacifico/Pacifico-Regular", 24);
	//LoadFont("chunk/Chunk", 24);
	game.font_title = LoadFont("pacifico/Pacifico-Bold", 80);
	game.font_small = LoadFont("league_gothic/LeagueGothic-Regular", 21);
	game.font_gothic_medium = LoadFont("league_gothic/LeagueGothic-Regular", 32);
	game.font_chunk = LoadFont("chunk/Chunk", 24);

	game.menu_earth = LoadImage("clouds/earth");

	game_skymanager = new SkyManager();
}

void GameDisplayFrame()
{
	DrawGameMenu();
}

void GameTimerFrame()
{
	switch(game.state)
	{
		case GAME_STATE_MENU:
			game_skymanager->Simulate();
			break;

		case GAME_STATE_PAUSED:break;

		default:
			game_skymanager->Simulate();
			break;
	}
}

void ShutdownGame()
{}

