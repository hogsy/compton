// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace ct
{
	struct Sprite
	{
		inline Sprite() {}
		inline Sprite( uint16_t _width, uint16_t _height ) : width( _width ), height( _height ) {}
		inline Sprite( uint16_t _width, uint16_t _height, std::vector< uint8_t > _pixels ) : width( _width ), height( _height ), pixels( _pixels ) {}

		void Draw( int x, int y, bool alpha = true, bool mirror = false ) const;

		uint16_t               width{ 0 };
		uint16_t               height{ 0 };
		std::vector< uint8_t > pixels;

		bool hasAlpha{ false };
	};
}// namespace vc
