// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

class Camera;
class Background
{
public:
	Background();
	~Background();

	static constexpr unsigned int SPRITE_SETS = 128;

	// If a sprite doesn't match this size, it's invalid
	static constexpr unsigned int SPRITE_WIDTH  = 80;
	static constexpr unsigned int SPRITE_HEIGHT = 40;

	// How many sprites make up a block
	static constexpr unsigned int NUM_BLOCK_SPRITE_COLUMNS = 4;
	static constexpr unsigned int NUM_BLOCK_SPRITE_ROWS    = 4;
	static constexpr unsigned int NUM_BLOCK_SPRITES        = NUM_BLOCK_SPRITE_COLUMNS * NUM_BLOCK_SPRITE_ROWS;

	// Background is handled as "blocks", made up of tiles
	static constexpr unsigned int BLOCK_WIDTH       = 320;
	static constexpr unsigned int BLOCK_HEIGHT      = 160;
	static constexpr unsigned int NUM_BLOCK_COLUMNS = 32;
	static constexpr unsigned int NUM_BLOCK_ROWS    = 4;
	static constexpr unsigned int NUM_BLOCKS        = NUM_BLOCK_COLUMNS * NUM_BLOCK_ROWS;

	// Total size of the background in pixels
	static constexpr unsigned int PIXEL_WIDTH  = BLOCK_WIDTH * NUM_BLOCK_COLUMNS;
	static constexpr unsigned int PIXEL_HEIGHT = BLOCK_HEIGHT * NUM_BLOCK_ROWS;
	static constexpr unsigned int CENTER_X     = PIXEL_WIDTH / 2;
	static constexpr unsigned int CENTER_Y     = PIXEL_HEIGHT / 2;

	void Draw( const Camera &camera );

private:
	void DrawBlock( const Camera &camera, unsigned int block, int x, int y );
};
