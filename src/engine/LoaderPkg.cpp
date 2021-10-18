/* Copyright (C) 2020 Mark Sowden <markelswo@gmail.com>
 * Project Yin
 * */

#include "../shared.h"
#include "LoaderPkg.h"

uint8_t *Pkg_OpenFile( PLFile *file, PLPackageIndex *index )
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

PLPackage *Pkg_LoadPackage( const char *path )
{
	PLFile *filePtr = PlOpenFile( path, false );
	if ( filePtr == NULL )
	{
		Warning( "Failed to open package \"%s\"!\nPL: %s\n", path, PlGetError() );
		return NULL;
	}

	/* read in the header */
	char identifier[ 4 ];
	if ( PlReadFile( filePtr, identifier, 1, sizeof( identifier ) ) != sizeof( identifier ) )
	{
		PlCloseFile( filePtr );
		Warning( "Failed to read in identifier for \"%s\"!\nPL: %s\n", path, PlGetError() );
		return NULL;
	}

	if ( !( identifier[ 0 ] == 'P' && identifier[ 1 ] == 'K' && identifier[ 2 ] == 'G' && identifier[ 3 ] == '1' ) )
	{
		Error( "Invalid package header, \"%s\", expected \"PKG2\"!\n", identifier );
	}

	bool	 status;
	uint32_t numFiles = PlReadInt32( filePtr, false, &status );
	if ( !status )
	{
		Error( "Failed to read in the number of files within the \"%s\" package!\nPL: %s\n", path, PlGetError() );
	}

	PLPackage *package = PlCreatePackageHandle( path, numFiles, Pkg_OpenFile );
	for ( uint32_t i = 0; i < numFiles; ++i )
	{
		PLPackageIndex *index = &package->table[ i ];

		/* read in the filename, it's a sized string... */
		uint8_t nameLength = PlReadInt8( filePtr, &status );
		if ( PlReadFile( filePtr, index->fileName, sizeof( char ), nameLength ) != nameLength )
		{
			Error( "Failed to read in filename within the \"%s\" package!\nPL: %s\n", path, PlGetError() );
		}

		/* file length/size */
		index->fileSize = PlReadInt32( filePtr, false, &status );
		index->offset	= PlGetFileOffset( filePtr );

		/* now seek to the next file */
		if ( !PlFileSeek( filePtr, index->fileSize, PL_SEEK_CUR ) )
		{
			Error( "Failed to seek to the next file within the \"%s\" package!\nPL: %s\n", path, PlGetError() );
		}

		Print( " Registered %s\n", index->fileName );
	}

	PlCloseFile( filePtr );

	return package;
}
