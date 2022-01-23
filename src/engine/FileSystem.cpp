// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

static void *FS_fopen( const char *path, const char *mode )
{
	DebugMsg( "%s %s %s\n", __FUNCTION__, path, mode );
	SgAssert( *mode != 'w', "Unsupported operation in FS_fopen!\n" );
	PLFile *file = PlOpenFile( path, false );
	if ( file == nullptr )
	{
		Warning( "Failed to open \"%s\"!\nPL: %s\n", path, PlGetError() );
		return nullptr;
	}

	return file;
}

static bool FS_fclose( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	PlCloseFile( file );
	return true;
}

static size_t FS_fread( ALLEGRO_FILE *f, void *ptr, size_t size )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return PlReadFile( file, ptr, 1, size );
}

static int64_t FS_ftell( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return PlGetFileOffset( file );
}

static bool FS_fseek( ALLEGRO_FILE *f, int64_t offset, int whence )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return PlFileSeek( file, offset, static_cast< PLFileSeek >( whence ) );
}

static bool FS_feof( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return PlIsEndOfFile( file );
}

static int FS_ferror( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	return PlGetFunctionResult();
}

static const char *FS_ferrmsg( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	return PlGetError();
}

static void FS_fclearerr( ALLEGRO_FILE *f )
{
	DebugMsg( "%s\n", __FUNCTION__ );
	PlUnused( f );
}

static int FS_fungetc( ALLEGRO_FILE *f, int c )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	return c;
}

static off_t FS_fsize( ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return PlGetFileSize( file );
}

ALLEGRO_FILE_INTERFACE g_fsIOInterface = {
		FS_fopen,
		FS_fclose,
		FS_fread,
		nullptr, /* fwrite */
		nullptr, /* fflush */
		FS_ftell,
		FS_fseek,
		FS_feof,
		FS_ferror,
		FS_ferrmsg,
		FS_fclearerr,
		FS_fungetc,
		FS_fsize,
};

/////////////////////////////////////////////////////////////////

/// Attempt to load the file provided by path into a null-terminated
/// buffer.
/// \param path Path to the file to load.
/// \param length Returns the length of the allocated buffer that's returned.
/// \return
char *vc::fs::LoadFileIntoBuffer( const char *path, unsigned int *length )
{
	PLFile *file = PlOpenFile( path, true );
	if ( file == nullptr )
	{
		Warning( "Failed to open file: %s\n", PlGetError() );
		return nullptr;
	}

	unsigned int fileSize = PlGetFileSize( file );
	char *buffer = new char[ fileSize + 1 ];
	unsigned int bytesRead = PlReadFile( file, buffer, 1, fileSize );
	if ( bytesRead != fileSize )
	{
		Warning( "Failed to read file: %s\n", PlGetError() );
		delete[] buffer;
		buffer = nullptr;
	}
	else
	{
		buffer[ fileSize ] = '\0';
	}

	PlCloseFile( file );

	if ( length != nullptr )
	{
		*length = ( buffer != nullptr ) ? fileSize : 0;
	}

	return buffer;
}
