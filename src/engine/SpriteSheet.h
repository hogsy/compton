// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "ScriptParser.h"
#include "Sprite.h"

typedef struct PLImage PLImage;

namespace vc
{
	class SpriteSheet : public ScriptParser
	{
	public:
		SpriteSheet( const char *path, const PLImage *bitmap );
		~SpriteSheet();

		const Sprite *GetSprite( const char *spriteName ) const;

	private:
		std::map< std::string, Sprite > sprites;
	};
}// namespace vc
