/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Compton.h"
#include "GUIStyleSheet.h"

vc::GUIStyleSheet::GUIStyleSheet( const char *path, ALLEGRO_BITMAP *bitmap ) : SpriteSheet( path, bitmap )
{
	// Outset
	GetSpriteCoordinates( "gui_lu", &outset.lu.x, &outset.lu.y, &outset.lu.w, &outset.lu.h );
	GetSpriteCoordinates( "gui_ru", &outset.ru.x, &outset.ru.y, &outset.ru.w, &outset.ru.h );
	GetSpriteCoordinates( "gui_ll", &outset.ll.x, &outset.ll.y, &outset.ll.w, &outset.ll.h );
	GetSpriteCoordinates( "gui_lr", &outset.lr.x, &outset.lr.y, &outset.lr.w, &outset.lr.h );
	GetSpriteCoordinates( "gui_u", &outset.u.x, &outset.u.y, &outset.u.w, &outset.u.h );
	GetSpriteCoordinates( "gui_r", &outset.r.x, &outset.r.y, &outset.r.w, &outset.r.h );
	GetSpriteCoordinates( "gui_d", &outset.d.x, &outset.d.y, &outset.d.w, &outset.d.h );
	GetSpriteCoordinates( "gui_l", &outset.l.x, &outset.l.y, &outset.l.w, &outset.l.h );

	// Inset
	GetSpriteCoordinates( "gui_ilu", &inset.lu.x, &inset.lu.y, &inset.lu.w, &inset.lu.h );
	GetSpriteCoordinates( "gui_iru", &inset.ru.x, &inset.ru.y, &inset.ru.w, &inset.ru.h );
	GetSpriteCoordinates( "gui_ill", &inset.ll.x, &inset.ll.y, &inset.ll.w, &inset.ll.h );
	GetSpriteCoordinates( "gui_ilr", &inset.lr.x, &inset.lr.y, &inset.lr.w, &inset.lr.h );
	GetSpriteCoordinates( "gui_iu", &inset.u.x, &inset.u.y, &inset.u.w, &inset.u.h );
	GetSpriteCoordinates( "gui_ir", &inset.r.x, &inset.r.y, &inset.r.w, &inset.r.h );
	GetSpriteCoordinates( "gui_id", &inset.d.x, &inset.d.y, &inset.d.w, &inset.d.h );
	GetSpriteCoordinates( "gui_il", &inset.l.x, &inset.l.y, &inset.l.w, &inset.l.h );

	// Backgrounds
	numBackgrounds = 0;
	for ( unsigned int i = 0; i < GUI_MAX_BACKGROUNDS; ++i )
	{
		char bgName[ 32 ];
		snprintf( bgName, sizeof( bgName ), "gui_bg%d", i );

		RectangleCoord background;
		if ( !GetSpriteCoordinates( bgName, &background.x, &background.y, &background.w, &background.h ) )
		{
			break;
		}

		backgrounds[ i ] = background;
		numBackgrounds++;
	}

	// Mouse cursor
	GetSpriteCoordinates( "gui_mouse_pointer", &mouseStyles[ GUI_MOUSE_DEFAULT ].x, &mouseStyles[ GUI_MOUSE_DEFAULT ].y, &mouseStyles[ GUI_MOUSE_DEFAULT ].w, &mouseStyles[ GUI_MOUSE_DEFAULT ].h );
	GetSpriteCoordinates( "gui_mouse_deny", &mouseStyles[ GUI_MOUSE_DENY ].x, &mouseStyles[ GUI_MOUSE_DENY ].y, &mouseStyles[ GUI_MOUSE_DENY ].w, &mouseStyles[ GUI_MOUSE_DENY ].h );
	GetSpriteCoordinates( "gui_mouse_move", &mouseStyles[ GUI_MOUSE_MOVE ].x, &mouseStyles[ GUI_MOUSE_MOVE ].y, &mouseStyles[ GUI_MOUSE_MOVE ].w, &mouseStyles[ GUI_MOUSE_MOVE ].h );
	GetSpriteCoordinates( "gui_mouse_sizer0", &mouseStyles[ GUI_MOUSE_SIZER0 ].x, &mouseStyles[ GUI_MOUSE_SIZER0 ].y, &mouseStyles[ GUI_MOUSE_SIZER0 ].w, &mouseStyles[ GUI_MOUSE_SIZER0 ].h );
	GetSpriteCoordinates( "gui_mouse_sizer1", &mouseStyles[ GUI_MOUSE_SIZER1 ].x, &mouseStyles[ GUI_MOUSE_SIZER1 ].y, &mouseStyles[ GUI_MOUSE_SIZER1 ].w, &mouseStyles[ GUI_MOUSE_SIZER1 ].h );
	GetSpriteCoordinates( "gui_mouse_sizer2", &mouseStyles[ GUI_MOUSE_SIZER2 ].x, &mouseStyles[ GUI_MOUSE_SIZER2 ].y, &mouseStyles[ GUI_MOUSE_SIZER2 ].w, &mouseStyles[ GUI_MOUSE_SIZER2 ].h );
	GetSpriteCoordinates( "gui_mouse_sizer3", &mouseStyles[ GUI_MOUSE_SIZER3 ].x, &mouseStyles[ GUI_MOUSE_SIZER3 ].y, &mouseStyles[ GUI_MOUSE_SIZER3 ].w, &mouseStyles[ GUI_MOUSE_SIZER3 ].h );
}
