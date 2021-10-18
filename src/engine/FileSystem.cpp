/*
SimGame Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
