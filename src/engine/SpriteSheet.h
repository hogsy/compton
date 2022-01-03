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
		bool ParseFile( const char *buffer, unsigned int bufferSize );

		virtual void SetupElementTable() {}

	public:
		const Sprite *LookupElement( const char *spriteName ) const;

	private:
		std::map< std::string, const Sprite * > elements_;
	};
}// namespace vc
