// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GUIStyleSheet.h"

vc::GUIStyleSheet::GUIStyleSheet( const char *path, const PLImage *bitmap ) : SpriteSheet( path, bitmap )
{
	// Outset
	frameSprites[ GUI_FRAME_UL ] = GetSprite( "gui_ul" );
	frameSprites[ GUI_FRAME_UR ] = GetSprite( "gui_ur" );
	frameSprites[ GUI_FRAME_LL ] = GetSprite( "gui_ll" );
	frameSprites[ GUI_FRAME_LR ] = GetSprite( "gui_lr" );
	frameSprites[ GUI_FRAME_U ]  = GetSprite( "gui_u" );
	frameSprites[ GUI_FRAME_R ]  = GetSprite( "gui_r" );
	frameSprites[ GUI_FRAME_D ]  = GetSprite( "gui_d" );
	frameSprites[ GUI_FRAME_L ]  = GetSprite( "gui_l" );

	// Inset
	frameSprites[ GUI_FRAME_IUL ] = GetSprite( "gui_iul" );
	frameSprites[ GUI_FRAME_IUR ] = GetSprite( "gui_iur" );
	frameSprites[ GUI_FRAME_ILL ] = GetSprite( "gui_ill" );
	frameSprites[ GUI_FRAME_ILR ] = GetSprite( "gui_ilr" );
	frameSprites[ GUI_FRAME_IU ]  = GetSprite( "gui_iu" );
	frameSprites[ GUI_FRAME_IR ]  = GetSprite( "gui_ir" );
	frameSprites[ GUI_FRAME_ID ]  = GetSprite( "gui_id" );
	frameSprites[ GUI_FRAME_IL ]  = GetSprite( "gui_il" );

	// Backgrounds
	numBackgrounds = 0;
	for ( unsigned int i = 0; i < GUI_MAX_BACKGROUNDS; ++i )
	{
		char bgName[ 32 ];
		snprintf( bgName, sizeof( bgName ), "gui_bg%d", i );
		backgrounds[ i ] = GetSprite( bgName );
		if ( backgrounds[ i ] == nullptr )
		{
			break;
		}

		numBackgrounds++;
	}

	// Mouse cursor
	mouseSprites[ vc::GUI_MOUSE_DEFAULT ] = GetSprite( "gui_mouse_pointer" );
	mouseSprites[ vc::GUI_MOUSE_DENY ]    = GetSprite( "gui_mouse_deny" );
	mouseSprites[ vc::GUI_MOUSE_MOVE ]    = GetSprite( "gui_mouse_move" );
	mouseSprites[ vc::GUI_MOUSE_SIZER0 ]  = GetSprite( "gui_mouse_sizer0" );
	mouseSprites[ vc::GUI_MOUSE_SIZER1 ]  = GetSprite( "gui_mouse_sizer1" );
	mouseSprites[ vc::GUI_MOUSE_SIZER2 ]  = GetSprite( "gui_mouse_sizer2" );
	mouseSprites[ vc::GUI_MOUSE_SIZER3 ]  = GetSprite( "gui_mouse_sizer3" );
}
