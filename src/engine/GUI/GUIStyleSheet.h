/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "SpriteSheet.h"

namespace vc
{
	struct RectangleCoord
	{
		int x{ 0 }, y{ 0 };
		int w{ 0 }, h{ 0 };
	};

	enum GUICursorMode
	{
		GUI_MOUSE_DEFAULT,
		GUI_MOUSE_DENY,
		GUI_MOUSE_MOVE,

		GUI_MOUSE_SIZER0,
		GUI_MOUSE_SIZER1,
		GUI_MOUSE_SIZER2,
		GUI_MOUSE_SIZER3,

		MAX_MOUSE_STATES
	};

	struct GUIStyleSheet : public SpriteSheet
	{
		GUIStyleSheet( const char *path, ALLEGRO_BITMAP *bitmap );

		struct GUIBorderStyle
		{
			RectangleCoord lu;
			RectangleCoord ru;
			RectangleCoord ll;
			RectangleCoord lr;
			// Mid
			RectangleCoord u;
			RectangleCoord r;
			RectangleCoord d;
			RectangleCoord l;
		};

		GUIBorderStyle outset;
		GUIBorderStyle inset;

#define GUI_MAX_BACKGROUNDS 8
		RectangleCoord backgrounds[ GUI_MAX_BACKGROUNDS ];
		unsigned int   numBackgrounds{ 0 };

		// Mouse cursor
		RectangleCoord mouseStyles[ MAX_MOUSE_STATES ];
	};
}// namespace vc
