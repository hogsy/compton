/* Copyright (C) 2020 Mark Sowden <markelswo@gmail.com>
 * Project Yin
 * */

#include <miniz.h>

#include "../shared.h"
#include "LoaderPkg.h"

uint8_t *Pkg_OpenFile( PLFile *file, PLPackageIndex *index ) {
	if( !plFileSeek( file, (signed)index->offset, PL_SEEK_SET ) ) {
		return NULL;
	}

	uint8_t *data = new uint8_t[ index->compressedSize ];
	if ( plReadFile( file, data, index->compressedSize, 1 ) != 1 ) {
		delete[] data;
		return NULL;
	}

	if ( index->compressionType == PL_COMPRESSION_ZLIB ) {
		/* go ahead and decompress it */
		uint8_t *uncompressedData = new uint8_t[ index->fileSize ];
		unsigned long uncompressedLength;
		int status = mz_uncompress( uncompressedData, &uncompressedLength, data, index->compressedSize );

		/* don't need this anymore! */
		delete[] data;
		data = uncompressedData;

		if ( status != MZ_OK ) {
			free( uncompressedData );

			Warning( "Failed to decompress \"%s\" from package \"%s\"!\n", index->fileName, plGetFilePath( file ) );
			return NULL;
		}
	}

	return data;
}

PLPackage *Pkg_LoadPackage( const char *path ) {
	PLFile *filePtr = plOpenFile( path, false );
	if ( filePtr == NULL ) {
		Warning( "Failed to open package \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	/* read in the header */
	char identifier[ 4 ];
	if( plReadFile( filePtr, identifier, 1, sizeof( identifier ) ) != sizeof( identifier ) ) {
		plCloseFile( filePtr );
		Warning( "Failed to read in identifier for \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	if( !( identifier[ 0 ] == 'P' && identifier[ 1 ] == 'K' && identifier[ 2 ] == 'G' && identifier[ 3 ] == '2' ) ) {
		Error( "Invalid package header, \"%s\", expected \"PKG2\"!\n", identifier );
	}

	bool status;
	uint32_t numFiles = plReadInt32( filePtr, false, &status );
	if ( !status ) {
		Error( "Failed to read in the number of files within the \"%s\" package!\nPL: %s\n", path, plGetError() );
	}

	PLPackage *package = plCreatePackageHandle( path, numFiles, Pkg_OpenFile );
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
		index->compressedSize = plReadInt32( filePtr, false, &status );
		if ( !status ) {
			Error( "Failed to read in the file sizes for \"%s\" within the \"%s\" package!\nPL: %s\n", index->fileName, path, plGetError() );
		}

		if ( index->fileSize != index->compressedSize ) {
			index->compressionType = PL_COMPRESSION_ZLIB;
		}

		index->offset = plGetFileOffset( filePtr );

		/* now seek to the next file */
		if ( !plFileSeek( filePtr, index->compressedSize, PL_SEEK_CUR ) ) {
			Error( "Failed to seek to the next file within the \"%s\" package!\nPL: %s\n", path, plGetError() );
		}

		/* PrintMsg( " Registered %s\n", index->fileName ); */
	}

	plCloseFile( filePtr );

	return package;
}
