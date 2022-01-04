// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "SpriteSheet.h"
#include "GUICursor.h"

namespace vc
{
	class GUIStyleSheet : public SpriteSheet
	{
	public:
		GUIStyleSheet() = default;

		static constexpr unsigned int GUI_MAX_BACKGROUNDS = 8;

		enum
		{
			// Outset
			GUI_FRAME_UL,// Upper Left
			GUI_FRAME_UR,// Upper Right
			GUI_FRAME_LL,// Lower Left
			GUI_FRAME_LR,// Lower Right
			GUI_FRAME_U, // Upper
			GUI_FRAME_R, // Right
			GUI_FRAME_D, // Down
			GUI_FRAME_L, // Left

			// Inset
			GUI_FRAME_IUL,// Upper Left
			GUI_FRAME_IUR,// Upper Right
			GUI_FRAME_ILL,// Lower Left
			GUI_FRAME_ILR,// Lower Right
			GUI_FRAME_IU, // Upper
			GUI_FRAME_IR, // Right
			GUI_FRAME_ID, // Down
			GUI_FRAME_IL, // Left

			GUI_MAX_FRAME_ELEMENTS
		};

		const Sprite *mouseSprites[ MAX_MOUSE_STATES ];
		const Sprite *frameSprites[ GUI_MAX_FRAME_ELEMENTS ];
		const Sprite *backgrounds[ GUI_MAX_BACKGROUNDS ];
		unsigned int  numBackgrounds{ 0 };

	private:
		inline void SetupElementTable() override
		{
			// Outset
			frameSprites[ GUI_FRAME_UL ] = LookupElement( "gui_ul" );
			frameSprites[ GUI_FRAME_UR ] = LookupElement( "gui_ur" );
			frameSprites[ GUI_FRAME_LL ] = LookupElement( "gui_ll" );
			frameSprites[ GUI_FRAME_LR ] = LookupElement( "gui_lr" );
			frameSprites[ GUI_FRAME_U ]  = LookupElement( "gui_u" );
			frameSprites[ GUI_FRAME_R ]  = LookupElement( "gui_r" );
			frameSprites[ GUI_FRAME_D ]  = LookupElement( "gui_d" );
			frameSprites[ GUI_FRAME_L ]  = LookupElement( "gui_l" );

			// Inset
			frameSprites[ GUI_FRAME_IUL ] = LookupElement( "gui_iul" );
			frameSprites[ GUI_FRAME_IUR ] = LookupElement( "gui_iur" );
			frameSprites[ GUI_FRAME_ILL ] = LookupElement( "gui_ill" );
			frameSprites[ GUI_FRAME_ILR ] = LookupElement( "gui_ilr" );
			frameSprites[ GUI_FRAME_IU ]  = LookupElement( "gui_iu" );
			frameSprites[ GUI_FRAME_IR ]  = LookupElement( "gui_ir" );
			frameSprites[ GUI_FRAME_ID ]  = LookupElement( "gui_id" );
			frameSprites[ GUI_FRAME_IL ]  = LookupElement( "gui_il" );

			// Backgrounds
			numBackgrounds = 0;
			for ( unsigned int i = 0; i < GUI_MAX_BACKGROUNDS; ++i )
			{
				char bgName[ 32 ];
				snprintf( bgName, sizeof( bgName ), "gui_bg%d", i );
				backgrounds[ i ] = LookupElement( bgName );
				if ( backgrounds[ i ] == nullptr )
				{
					break;
				}

				numBackgrounds++;
			}

			// Mouse cursor
			mouseSprites[ GUI_MOUSE_DEFAULT ] = LookupElement( "gui_mouse_pointer" );
			mouseSprites[ GUI_MOUSE_DENY ]    = LookupElement( "gui_mouse_deny" );
			mouseSprites[ GUI_MOUSE_MOVE ]    = LookupElement( "gui_mouse_move" );
			mouseSprites[ GUI_MOUSE_SIZER0 ]  = LookupElement( "gui_mouse_sizer0" );
			mouseSprites[ GUI_MOUSE_SIZER1 ]  = LookupElement( "gui_mouse_sizer1" );
			mouseSprites[ GUI_MOUSE_SIZER2 ]  = LookupElement( "gui_mouse_sizer2" );
			mouseSprites[ GUI_MOUSE_SIZER3 ]  = LookupElement( "gui_mouse_sizer3" );
		}
	};
}// namespace vc
