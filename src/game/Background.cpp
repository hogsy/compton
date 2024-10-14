// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Background.cpp
 *  Drawing SFC style background.
 *------------------------------------------------------------------------------------*/

#include "Background.h"

#include "../../engine/Camera.h"

vc::Background::Background()  = default;
vc::Background::~Background() = default;

void vc::Background::Draw( const Camera &camera )
{
	for ( unsigned int i = 0, block = 0; i < NUM_BLOCK_ROWS; ++i )
	{
		for ( unsigned int j = 0; j < NUM_BLOCK_COLUMNS; ++j )
		{
			DrawBlock( camera, block++, BLOCK_WIDTH * j, BLOCK_HEIGHT * i );
		}
	}
}

void vc::Background::DrawBlock( const Camera &camera, unsigned int block, int x, int y )
{
	int sw = GetApp()->GetDrawWidth();
	int sh = GetApp()->GetDrawHeight();

	if ( x > camera.position.x + sw || x + BLOCK_WIDTH < camera.position.x - sw ||
	     y > camera.position.y + sh || y + BLOCK_HEIGHT < camera.position.y - sh )
	{
		return;
	}

	// Transform
	x -= ( int ) camera.position.x;
	y -= ( int ) camera.position.y;

	int nx = x;
	int ny = y;
	for ( unsigned int i = 0, sprite = 0; i < NUM_BLOCK_SPRITE_ROWS; ++i )
	{
		for ( unsigned int j = 0; j < NUM_BLOCK_SPRITE_COLUMNS; ++j )
		{
			GetApp()->GetImageManager()->DrawSprite( block, sprite++, nx, ny );
			nx += SPRITE_WIDTH;
		}
		nx = x;
		ny += SPRITE_HEIGHT;
	}
}
