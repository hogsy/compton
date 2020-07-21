/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "GUIStyleSheet.h"

vc::GUIStyleSheet::GUIStyleSheet( const char *path, ALLEGRO_BITMAP *bitmap ) : SpriteSheet( path, bitmap ) {
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
	for ( unsigned int i = 0; i < GUI_MAX_BACKGROUNDS; ++i ) {
		char bgName[ 32 ];
		snprintf( bgName, sizeof( bgName ), "gui_bg%d", i );

		RectangleCoord background;
		if ( !GetSpriteCoordinates( bgName, &background.x, &background.y, &background.w, &background.h ) ) {
			break;
		}

		backgrounds[ i ] = background;
		numBackgrounds++;
	}

	// Mouse cursor
	GetSpriteCoordinates( "gui_mouse_pointer", &mousePointer.x, &mousePointer.y, &mousePointer.w, &mousePointer.h );
	GetSpriteCoordinates( "gui_mouse_deny", &mouseDeny.x, &mouseDeny.y, &mouseDeny.w, &mouseDeny.h );
	GetSpriteCoordinates( "gui_mouse_move", &mouseMove.x, &mouseMove.y, &mouseMove.w, &mouseMove.h );
	GetSpriteCoordinates( "gui_mouse_sizer0", &mouseSizer0.x, &mouseSizer0.y, &mouseSizer0.w, &mouseSizer0.h );
	GetSpriteCoordinates( "gui_mouse_sizer1", &mouseSizer1.x, &mouseSizer1.y, &mouseSizer1.w, &mouseSizer1.h );
	GetSpriteCoordinates( "gui_mouse_sizer2", &mouseSizer2.x, &mouseSizer2.y, &mouseSizer2.w, &mouseSizer2.h );
	GetSpriteCoordinates( "gui_mouse_sizer3", &mouseSizer3.x, &mouseSizer3.y, &mouseSizer3.w, &mouseSizer3.h );
}
