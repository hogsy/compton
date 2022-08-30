// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"
#include "LoaderPkg.h"

void *Pkg_OpenFile( PLFile *file, PLPackageIndex *index )
{
	if ( !PlFileSeek( file, ( signed ) index->offset, PL_SEEK_SET ) )
	{
		return NULL;
	}

	uint8_t *data = new uint8_t[ index->fileSize ];
	if ( PlReadFile( file, data, index->fileSize, 1 ) != 1 )
	{
		delete[] data;
		return NULL;
	}

	return data;
}

static PLPackage *ParsePackage( PLFile *file, const char *path )
{
	uint32_t magic = PlReadInt32( file, false, NULL );
	static constexpr unsigned int pkgMagic = PL_MAGIC_TO_NUM( 'P', 'K', 'G', '1' );
	if ( magic != pkgMagic )
	{
		Warning( "Invalid package header, \"%s\", expected \"PKG2\"!\n", magic );
		return nullptr;
	}

	bool status;
	uint32_t numFiles = PlReadInt32( file, false, &status );
	if ( !status )
	{
		Warning( "Failed to read in the number of files within the \"%s\" package: %s\n", path, PlGetError() );
		return nullptr;
	}

	PLPackage *package = PlCreatePackageHandle( path, numFiles, Pkg_OpenFile );
	for ( uint32_t i = 0; i < numFiles; ++i )
	{
		PLPackageIndex *index = &package->table[ i ];

		/* read in the filename, it's a sized string... */
		uint8_t nameLength = PlReadInt8( file, &status );
		if ( !status )
		{
			break;
		}

		if ( PlReadFile( file, index->fileName, sizeof( char ), nameLength ) != nameLength )
		{
			status = false;
			Warning( "Failed to read in filename within the \"%s\" package: %s\n", path, PlGetError() );
			break;
		}

		/* file length/size */
		index->fileSize = PlReadInt32( file, false, &status );
		if ( !status )
		{
			break;
		}

		index->offset = PlGetFileOffset( file );

		/* now seek to the next file */
		if ( !PlFileSeek( file, index->fileSize, PL_SEEK_CUR ) )
		{
			status = false;
			Warning( "Failed to seek to the next file within the \"%s\" package: %s\n", path, PlGetError() );
			break;
		}

		Print( " Registered %s\n", index->fileName );
	}

	if ( !status )
	{
		Warning( "Failed to parse package: %s\n", path );
		PlDestroyPackage( package );
		package = nullptr;
	}

	return package;
}

PLPackage *Pkg_LoadPackage( const char *path )
{
	PLFile *filePtr = PlOpenFile( path, false );
	if ( filePtr == nullptr )
	{
		Warning( "Failed to open package \"%s\"!\nPL: %s\n", path, PlGetError() );
		return nullptr;
	}

	PLPackage *package = ParsePackage( filePtr, path );

	PlCloseFile( filePtr );

	return package;
}
