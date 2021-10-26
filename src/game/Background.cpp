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
/*--------------------------------------------------------------------------------------
 * Background.cpp
 *  Drawing SFC style background.
 *------------------------------------------------------------------------------------*/

#include "Background.h"
#include "Camera.h"

vc::Background::Background()
{
}

vc::Background::~Background()
{
}

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
			GetApp()->GetImageManager()->DrawSprite( block, sprite++, nx, ny );
			nx += SPRITE_WIDTH;
		}
		nx = x;
		ny += SPRITE_HEIGHT;
	}
}
