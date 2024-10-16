// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * Background.cpp
 *  Drawing SFC style background.
 *------------------------------------------------------------------------------------*/

#include "app.h"
#include "background.h"
#include "camera.h"

Background::Background()  = default;
Background::~Background() = default;

bool Background::load( const std::string &path )
{
	if ( ( layers[ BACKGROUND_LAYER_COLLISION ] = ImageManager::CacheSprite( path + "_c.png" ) ) == nullptr )
	{
		return false;
	}

	// use collision as the baseline w/h
	width  = layers[ BACKGROUND_LAYER_COLLISION ]->width;
	height = layers[ BACKGROUND_LAYER_COLLISION ]->height;

	layers[ BACKGROUND_LAYER_BACK ]  = ImageManager::CacheSprite( path + "_b.png" );
	layers[ BACKGROUND_LAYER_FRONT ] = ImageManager::CacheSprite( path + "_f.png" );

	return true;
}

void Background::draw( const Camera &camera )
{
	if ( layers[ BACKGROUND_LAYER_COLLISION ] == nullptr )
	{
		return;
	}

	// Transform
	int x = 0 - ( int ) camera.position.x;
	int y = 0 - ( int ) camera.position.y;

	layers[ BACKGROUND_LAYER_COLLISION ]->Draw( x, y, true );
}
