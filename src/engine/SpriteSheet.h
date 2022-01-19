// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Sprite.h"

namespace vc
{
	class SpriteSheet
	{
	public:
		SpriteSheet() = default;
		~SpriteSheet();

		bool LoadFile( const char *path );

	private:
		void ExtractSprite( const struct PLImage *image, const std::string &name, int x, int y, int w, int h );
		bool ParseSprite( const char **p, const PLImage *image );
		bool ParseGroup( const char **p, const PLImage *image );
		bool ParseFile( const char *buffer );

		virtual void SetupElementTable() {}

	public:
		const Sprite *LookupElement( const char *spriteName ) const;

	private:
		std::map< std::string, Sprite > elements_;
	};
}// namespace vc
