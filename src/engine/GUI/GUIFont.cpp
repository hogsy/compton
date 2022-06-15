// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_filesystem.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC

#define STBTT_malloc( x, u ) PlMAllocA( x )
#define STBTT_free( x, u )   PlFree( x )

#include "stb_truetype.h"

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
			stbtt_GetCodepointBitmapBox( &fontInfo, c, scale, scale, &w, &h, &xo, &yo );

			Glyph glyph;
			glyph.w = w;
			glyph.h = h;
			glyph.buffer.resize( w * h );

			stbtt_MakeCodepointBitmap( &fontInfo, glyph.buffer.data(), w, h, sizeof( unsigned char ), scale, scale, c );
		}
		status = true;
	}

	PlCloseFile( file );

	return status;
}
