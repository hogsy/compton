// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "ImageManager.h"

namespace vc
{
	namespace render
	{
		enum class FlipDirection
		{
			FLIP_NONE,
			FLIP_HORIZONTAL,
			FLIP_VERTICAL,
		};

		void BeginDraw();
		void EndDraw();

		void ClearDisplay();
		void DrawPixel( int x, int y, const hei::Colour &colour );
		void DrawLine( int sx, int sy, int ex, int ey, const hei::Colour &colour );
		void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection = vc::render::FlipDirection::FLIP_NONE );
		void DrawBitmap( const uint8_t *pixels, uint8_t pixelSize, const ImageManager::Palette *palette, int x, int y, int w, int h, bool alphaTest, vc::render::FlipDirection flipDirection = vc::render::FlipDirection::FLIP_NONE );
		void DrawBitmapRegion( const uint8_t *pixels, int x, int y, int w, int h, int dx, int dy, int dw, int dh, bool alphaTest = false );
		void DrawFilledRectangle( int x, int y, int w, int h, const hei::Colour &colour );

		void DrawPanel( int x, int y, int w, int h, const hei::Colour &colour );

		void SetScissor( int x, int y, int w, int h );

		bool IsVolumeVisible( int x, int y, int w, int h );

		struct DrawStats
		{
			enum Type : uint8_t
			{
				DRAW_STATS_ENTITY,
				DRAW_STATS_TILE,

				MAX_DRAW_STATS_TYPES
			};

			unsigned int              stats[ Type::MAX_DRAW_STATS_TYPES ];
			inline static const char *GetDescription( Type type )
			{
				switch ( type )
				{
					case DRAW_STATS_ENTITY:
						return "entities";
					case DRAW_STATS_TILE:
						return "tiles";
					default:
						break;
				}

				assert( 0 );
				return "unnamed";
			}

			inline void Clear()
			{
				PL_ZERO( this, sizeof( DrawStats ) );
			}
		};
		extern DrawStats drawStats;
	}// namespace render

	// Common misc types

	struct RGBA8
	{
		uint8_t r, g, b, a;
	};
}// namespace vc
