// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"

#include <plcore/pl_filesystem.h>

#include "GUIFont.h"

bool ct::GUIFont::LoadFile( const char *path, float scale )
{
	PLFile *file = PlOpenFile( path, true );
	if ( file == nullptr )
		return false;

	unsigned char *data = ( unsigned char * ) PlGetFileData( file );

	bool status = false;
	stbtt_fontinfo fontInfo;
	if ( stbtt_InitFont( &fontInfo, data, 0 ) != 0 )
	{
		for ( int c = 0; c < 128; ++c )
		{
			int w, h;
			int xo, yo;
			unsigned char *bitmap = stbtt_GetGlyphBitmap(
					&fontInfo,
					scale, scale,
					c,
					&w, &h,
					&xo, &yo
					);
			assert( bitmap != nullptr );

			stbtt_GetGlyphBox()

			Glyph glyph;
			glyph.w = w;
			glyph.h = h;

		}
		stbtt_GetGlyphBitmap()
	}

	PlCloseFile( file );

	return status;
}
