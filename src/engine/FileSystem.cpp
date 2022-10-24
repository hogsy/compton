// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"
#include "FileSystem.h"


static void *FS_fopen( const char *path, const char *mode )
{
	DebugMsg( "%s %s %s\n", __FUNCTION__, path, mode );
	assert( *mode != 'w' );
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

/**
 * Attempt to load the file provided by path into a null-terminated buffer.
 */
char *ct::fs::LoadFileIntoBuffer( const char *path, unsigned int *length )
{
	PLFile *file = PlOpenFile( path, true );
	if ( file == nullptr )
	{
		Warning( "Failed to open file: %s\n", PlGetError() );
		return nullptr;
	}

	unsigned int fileSize  = PlGetFileSize( file );
	char        *buffer    = new char[ fileSize + 1 ];
	unsigned int bytesRead = PlReadFile( file, buffer, 1, fileSize );
	if ( bytesRead != fileSize )
	{
		Warning( "Failed to read file: %s\n", PlGetError() );
		delete[] buffer;
		buffer = nullptr;
	}
	else
		buffer[ fileSize ] = '\0';

	PlCloseFile( file );

	if ( length != nullptr )
		*length = ( buffer != nullptr ) ? fileSize : 0;

	return buffer;
}

/**
 * Returns the local location where data is stored.
 * Returned path is intentionally not appended with a forward slash!
 */
const char *ct::fs::GetDataLocation()
{
#if 1
	if ( PlFileExists( "./Compton" PL_SYSTEM_EXE_EXTENSION ) )
		return "../../data";

	return ".";
#else// probably overengineered solution...
	static char *buf = nullptr;
	if ( buf != nullptr )
		return buf;

	const char *wd = PlGetWorkingDirectory();
	if ( wd == nullptr )
	{
		if ( PlFileExists( "./Compton" PL_SYSTEM_EXE_EXTENSION ) )
			return "../../data";

		return ".";
	}

	if ( ( strrchr( wd, '/' ) == nullptr ) && ( strrchr( wd, '\\' ) == nullptr ) )
	{
		Print( "No path for working directory, assuming current.\n" );
		return ".";
	}

	size_t l = strlen( wd ) + 1;
	buf      = PL_NEW_( char, l );
	memcpy( buf, wd, l - 1 );

	// Clean it up
	PlNormalizePath( buf, l );

	// Now scan through until we find the runtime location, then append it with the data location
	char *p = buf;
	while ( ( p = strchr( p + 1, '/' ) ) != nullptr )
	{
		if ( pl_strncasecmp( p, "/runtime/", 9 ) != 0 )
			continue;

		break;
	}

	if ( p == nullptr )
		return ".";

	*( p + 1 ) = '\0';
	strcat( p, "data" );

	return buf;
#endif
}
