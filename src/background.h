// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "image_manager.h"

class Camera;
class Background
{
public:
	Background();
	~Background();

	bool load( const std::string &path );
	void draw( const Camera &camera );

	[[nodiscard]] unsigned int get_width() const { return width; }
	[[nodiscard]] unsigned int get_height() const { return height; }

private:
	enum
	{
		BACKGROUND_LAYER_BACK,
		BACKGROUND_LAYER_FRONT,
		BACKGROUND_LAYER_COLLISION,

		BACKGROUND_LAYER_MAX
	};

	ImageManager::Sprite *layers[ BACKGROUND_LAYER_MAX ];

	unsigned int width;
	unsigned int height;
};
