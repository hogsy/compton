/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"

static void *FS_fopen( const char *path, const char *mode ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	SgAssert( mode[ 0 ] != 'w', "Unsupported operation in FS_fopen!\n" );
	PLFile *file = plOpenFile( path, false );
	if ( file == nullptr ) {
		Warning( "Failed to open \"%s\"!\nPL: %s\n", path, plGetError() );
		return nullptr;
	}

	return file;
}

static bool FS_fclose( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	plCloseFile( file );
	return true;
}

static size_t FS_fread( ALLEGRO_FILE *f, void *ptr, size_t size ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return plReadFile( file, ptr, 1, size );
}

static int64_t FS_ftell( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return plGetFileOffset( file );
}

static bool FS_fseek( ALLEGRO_FILE *f, int64_t offset, int whence ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return plFileSeek( file, offset, static_cast< PLFileSeek >( whence ) );
}

static bool FS_feof( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return plIsEndOfFile( file );
}

static int FS_ferror( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	return plGetFunctionResult();
}

static const char *FS_ferrmsg( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	return plGetError();
}

static void FS_fclearerr( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	plUnused( f );
}

static int FS_fungetc( ALLEGRO_FILE *f, int c ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	return c;
}

static off_t FS_fsize( ALLEGRO_FILE *f ) {
	DebugMsg( "%s\n", __FUNCTION__ );
	PLFile *file = static_cast< PLFile * >( al_get_file_userdata( f ) );
	return plGetFileSize( file );
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