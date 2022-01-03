// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Background.cpp
 *  Drawing SFC style background.
 *------------------------------------------------------------------------------------*/

#include "Background.h"
#include "Camera.h"

vc::Background::Background() = default;
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
#if 0
	if ( x > camera.position.x + DISPLAY_WIDTH || x + BLOCK_WIDTH < camera.position.x - DISPLAY_WIDTH ||
	     y > camera.position.y + DISPLAY_HEIGHT || y + BLOCK_HEIGHT < camera.position.y - DISPLAY_HEIGHT )
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
			spriteManager->DrawSprite( sprite++, block, nx, ny );
			nx += SPRITE_WIDTH;
		}
		nx = x;
		ny += SPRITE_HEIGHT;
	}
#endif
}
