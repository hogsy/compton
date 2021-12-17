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

#include "../shared.h"

#include "GUICursor.h"

vc::GUICursor::GUICursor( GUIPanel *parent, int x, int y, int w, int h )
	: GUIPanel( parent, x, y, w, h, Background::NONE, Border::NONE ) {}

vc::GUICursor::~GUICursor()
{
}

void vc::GUICursor::DrawBackground()
{
	if ( !isDrawing )
	{
		return;
	}

#if defined( GAME_TYPE_SFC )
	GetApp()->GetImageManager()->DrawSprite( ImageManager::SPR_GROUP_OBJECTS_0, ImageManager::SPR_CURSOR, x, y, true );
#else
	if ( myStyleSheet == nullptr )
	{
		return;
	}

	ALLEGRO_BITMAP *bmp = myStyleSheet->GetBitmap();
	if ( bmp == nullptr )
	{
		return;
	}

	float sx, sy, sw, sh;
	sx = myStyleSheet->mouseStyles[ currentCursorMode ].x;
	sy = myStyleSheet->mouseStyles[ currentCursorMode ].y;
	sw = myStyleSheet->mouseStyles[ currentCursorMode ].w;
	sh = myStyleSheet->mouseStyles[ currentCursorMode ].h;

	al_draw_bitmap_region( bmp, sx, sy, sw, sh, x, y, 0 );
#endif
}

void vc::GUICursor::Tick()
{
	GetApp()->GetCursorPosition( &x, &y );

	int       pX, pY, pW, pH;
	GUIPanel *panel = GetParent();
	if ( panel == nullptr )
	{
		return;
	}

	panel->GetPosition( &pX, &pY );
	panel->GetSize( &pW, &pH );

	// Check the cursor is in the bounds of the parent
	if ( x + w < pX || x > pX + pW || y + h < pY || y > pY + pH )
	{
		isDrawing = false;
	}
	else
	{
		isDrawing = true;
	}
}
