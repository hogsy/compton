// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "BitmapFont.h"

ct::BitmapFont::~BitmapFont()
{
#if 1// Need to jog my memory to see if this is even necessary...
	for ( auto &i : glyphs_ )
	{
		i.pixels.clear();
		i.pixels.shrink_to_fit();
	}

	glyphs_.clear();
	glyphs_.shrink_to_fit();
#endif
}

bool ct::BitmapFont::LoadFromImage( uint8_t cw, uint8_t ch, uint16_t start, const char *path )
{
	PLImage *image = PlLoadImage( path );
	if ( image == nullptr )
	{
		Warning( "Failed to load image: %s\n", PlGetError() );
		return false;
	}

	PLImageFormat format = PlGetImageFormat( image );
	if ( format == PL_IMAGEFORMAT_UNKNOWN )
	{
		Warning( "Unknown image format!\n" );
		return false;
	}

	unsigned int pixelSize = PlGetImageFormatPixelSize( format );
	if ( pixelSize == 0 )
	{
		Warning( "Invalid pixel size for charset!\n" );
		return false;
	}

	unsigned int colourCount = PlGetNumImageFormatChannels( format );
	if ( colourCount == 0 )
	{
		Warning( "Invalid colour count for charset!\n" );
		return false;
	}

	// This is dumb, but only support explicit pixel sizes relative to colour count for now
	if ( colourCount != pixelSize )
	{
		Warning( "Unsupported pixel/colour size for charset!\n" );
		return false;
	}

	const uint8_t *imageBuffer = PlGetImageData( image, 0 );
	if ( imageBuffer == nullptr )
	{
		Warning( "Failed to get image data!\n" );
		return false;
	}

	// Determine how many glyphs the image could support
	unsigned int w         = PlGetImageWidth( image );
	unsigned int h         = PlGetImageHeight( image );
	unsigned int numGlyphs = ( w / cw ) * ( h / ch );
	glyphs_.resize( numGlyphs );

	DebugMsg( "Pulling %u glyphs from image...\n", numGlyphs );

	cw_ = cw;
	ch_ = ch;
	for ( unsigned int i = 0; i < numGlyphs; ++i )
	{
		glyphs_[ i ].pixels.resize( cw_ * ch_ );

		unsigned int iw = w / cw;
		unsigned int ir = i / iw;
		unsigned int ic = i % iw;
		unsigned int ix = ic * cw;
		unsigned int iy = ir * ch;

		// Now we need to copy the glyph from the image's
		// pixel buffer into our glyph's pixel buffer
		for ( unsigned int y = 0; y < ch_; ++y )
		{
			for ( unsigned int x = 0; x < cw_; ++x )
			{
				glyphs_[ i ].pixels[ x + y * cw_ ] = imageBuffer[ ( ix + x + ( iy + y ) * w ) * pixelSize ];
			}
		}
	}

	PlDestroyImage( image );

	return true;
}

#if 0// Meh...
bool vc::BitmapFont::LoadFromPSF( const char *path )
{
	PLFile *file = PlOpenFile( path, false );
	if ( file == nullptr )
	{
		Warning( "Failed to open font file: %s\n", PlGetError() );
		return false;
	}

	struct PSFHeader
	{
		uint32_t magic;
		uint32_t version;
		uint32_t headerSize;
		uint32_t flags;
		uint32_t numGlyphs;
		uint32_t bytesPerGlyph;
		uint32_t height;
		uint32_t width;
	};

	static constexpr uint32_t PSF_MAGIC = 0x864ab572;

	PSFHeader header;
	PlReadFile( file, &header, sizeof( PSFHeader ), 1 );
	if ( header.magic != PSF_MAGIC )
	{
		Warning( "Invalid PSF magic!\n" );
		return false;
	}

	return false;
}
#endif

void ct::BitmapFont::DrawCharacter( int x, int y, unsigned char c, const hei::Colour &colour ) const
{
	if ( c >= glyphs_.size() )
		return;

	// Don't bother drawing it if it's outside visible bounds
	if ( x + cw_ < 0 || x > DISPLAY_WIDTH || y + ch_ < 0 || y > DISPLAY_HEIGHT )
		return;

	for ( unsigned int row = 0; row < cw_; ++row )
	{
		for ( unsigned int column = 0; column < ch_; ++column )
		{
			uint8_t pixel = glyphs_[ c ].pixels[ row + column * cw_ ];
			if ( pixel == 0 )
				continue;

#if 0
			render::DrawPixel( x + row, y + column, hei::Colour( pixel * colour.r, pixel * colour.g, pixel * colour.b ) );
#else
			render::DrawPixel( x + row, y + column, hei::Colour( colour.r, colour.g, colour.b ) );
#endif
		}
	}
}

void ct::BitmapFont::DrawString( int *x, int *y, const char *sentence, const hei::Colour &colour, bool shadow ) const
{
	if ( shadow )
	{
		int xx = *x + 1, yy = *y + 1;
		DrawString( &xx, &yy, sentence, hei::Colour( 0, 0, 0 ), false );
	}

	int                  ox = *x;
	const unsigned char *p  = ( unsigned char  *) sentence;
	while ( *p != '\0' )
	{
		if ( *p == '\n' )
		{
			*y += ch_;
			*x = ox;
			p++;
			continue;
		}
		else if ( *p == '\t' )
		{
			*x += ( cw_ * 2 );
			p++;
			continue;
		}

		DrawCharacter( *x, *y, *p++, colour );

		*x += cw_;
	}
}
