/* Copyright (C) 2020 Mark Sowden <markelswo@gmail.com>
 * Project Yin
 * */

#include "../shared.h"
#include "LoaderPkg.h"

PLPackage *Pkg_LoadPackage( const char *path ) {
	Print( "Loading %s...\n", path );

	PLFile *filePtr = plOpenFile( path, false );
	if ( filePtr == NULL ) {
		return NULL;
	}

	/* read in the header */
	char identifier[ 4 ];
	if( plReadFile( filePtr, identifier, 1, sizeof( identifier ) ) != sizeof( identifier ) ) {
		plCloseFile( filePtr );
		return NULL;
	}

	if( !( identifier[ 0 ] == 'P' && identifier[ 1 ] == 'K' && identifier[ 2 ] == 'G' && identifier[ 3 ] == '1' ) ) {
		Error( "Invalid package header, \"%s\", expected \"PKG1\"!\n", identifier );
	}

	bool status;
	uint32_t numFiles = plReadInt32( filePtr, false, &status );
	if ( !status ) {
		Error( "Failed to read in the number of files within the \"%s\" package!\nPL: %s\n", path, plGetError() );
	}

	PLPackage *package = plCreatePackageHandle( path, numFiles, NULL );
	for ( unsigned int i = 0; i < numFiles; ++i ) {
		PLPackageIndex *index = &package->table[ i ];

		/* read in the filename, it's a sized string... */
		uint8_t nameLength = plReadInt8( filePtr, &status );
		if ( plReadFile( filePtr, index->fileName, sizeof( char ), nameLength ) != nameLength ) {
			Error( "Failed to read in filename within the \"%s\" package!\nPL: %s\n", path, plGetError() );
		}

		index->fileName[ nameLength + 1 ] = '\0';

		/* file length/size */
		index->fileSize = plReadInt32( filePtr, false, &status );
		index->offset = plGetFileOffset( filePtr );

		/* now seek to the next file */
		if ( !plFileSeek( filePtr, index->fileSize, PL_SEEK_CUR ) ) {
			Error( "Failed to seek to the next file within the \"%s\" package!\nPL: %s\n", path, plGetError() );
		}

		Print( "Registered %s\n", index->fileName );
	}

	plCloseFile( filePtr );

	return package;
}
