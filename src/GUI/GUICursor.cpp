// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"
#include "../image_manager.h"

#include "GUICursor.h"

GUICursor::GUICursor( GUIPanel *parent, int x, int y, int w, int h )
    : GUIPanel( parent, x, y, w, h, Background::NONE, Border::NONE ) {}

GUICursor::~GUICursor() = default;

void GUICursor::DrawBackground()
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

void GUICursor::Tick()
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
