// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

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

static int FS_ferror( PL_UNUSED ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	return PlGetFunctionResult();
}

static const char *FS_ferrmsg( PL_UNUSED ALLEGRO_FILE *f )
{
	//DebugMsg( "%s\n", __FUNCTION__ );
	return PlGetError();
}

static void FS_fclearerr( PL_UNUSED ALLEGRO_FILE *f )
{
	DebugMsg( "%s\n", __FUNCTION__ );
}

static int FS_fungetc( PL_UNUSED ALLEGRO_FILE *f, int c )
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
