/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "SpriteSheet.h"

namespace vc {
	struct RectangleCoord {
		int x, y;
		int w, h;
	};

	struct GUIStyleSheet : public SpriteSheet {
		GUIStyleSheet( const char *path, ALLEGRO_BITMAP *bitmap );

		struct GUIBorderStyle {
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
		unsigned int numBackgrounds;

		// Mouse cursor
		RectangleCoord mousePointer;
		RectangleCoord mouseDeny;
		RectangleCoord mouseMove;
		RectangleCoord mouseSizer0;
		RectangleCoord mouseSizer1;
		RectangleCoord mouseSizer2;
		RectangleCoord mouseSizer3;
	};
}
