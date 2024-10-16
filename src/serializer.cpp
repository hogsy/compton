// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "app.h"
#include "serializer.h"

static const uint32_t storageVersion = 20200629;

Serializer::Serializer( const char *path, Serializer::Mode mode )
{
	filePtr = fopen( path, mode == Mode::READ ? "rb" : "wb" );
	if ( filePtr == nullptr )
	{
		Warning( "Failed to open \"%s\" for read/write operations, state will not be correctly restored!\n", path );
		return;
	}

	// If we're reading, make sure the dataset is good
	if ( mode == Mode::READ )
	{
		char identifier[ 4 ];
		fread( identifier, sizeof( char ), 4, filePtr );
		if ( identifier[ 0 ] != 'G' || identifier[ 1 ] != 'D' || identifier[ 2 ] != 'S' || identifier[ 3 ] != '1' )
		{
			Warning( "Invalid storage header, \"%s\", expected \"GDS1\"!\n", identifier );

			// Clear out the file so we don't do anything with it
			fclose( filePtr );
			filePtr = nullptr;
			return;
		}

		// Read in the version, we can use this to handle any incompatibilities etc.
		fread( &version, sizeof( uint32_t ), 1, filePtr );
	}
	else
	{
		// Otherwise, just write out the header
		fwrite( "GDS1", sizeof( char ), 4, filePtr );
		fwrite( &storageVersion, sizeof( uint32_t ), 1, filePtr );
	}
}

Serializer::~Serializer()
{
	if ( filePtr != nullptr )
	{
		fclose( filePtr );
	}
}

bool Serializer::ValidateDataFormat( uint8_t target )
{
	uint8_t format = fgetc( filePtr );
	if ( format != target )
	{
		Warning( "Invalid data format found, expected \"%d\" but got \"%d\"!\n", target, format );
		return false;
	}

	return true;
}

void Serializer::WriteInteger( int var )
{
	fputc( DATA_FORMAT_INTEGER, filePtr );

	fwrite( &var, sizeof( int32_t ), 1, filePtr );
}

void Serializer::WriteFloat( float var )
{
	fputc( DATA_FORMAT_FLOAT, filePtr );

	fwrite( &var, sizeof( float ), 1, filePtr );
}

void Serializer::WriteString( const char *var )
{
	fputc( DATA_FORMAT_STRING, filePtr );

	// Write out the length
	uint32_t length = strlen( var );
	fwrite( &length, sizeof( uint32_t ), 1, filePtr );

	// And now write out the string itself
	fwrite( var, sizeof( char ), length, filePtr );
}

void Serializer::WriteCoordinate( const PLVector2 &var )
{
	fputc( DATA_FORMAT_COORDINATE, filePtr );

	fwrite( &var, sizeof( PLVector2 ), 1, filePtr );
}

int Serializer::ReadInteger()
{
	if ( !ValidateDataFormat( DATA_FORMAT_INTEGER ) )
	{
		return 0;
	}

	int32_t var;
	fread( &var, sizeof( int32_t ), 1, filePtr );
	return var;
}

float Serializer::ReadFloat()
{
	if ( !ValidateDataFormat( DATA_FORMAT_FLOAT ) )
	{
		return 0.0f;
	}

	float var;
	fread( &var, sizeof( float ), 1, filePtr );
	return var;
}

void Serializer::ReadString( char *buffer, size_t bufLength )
{
	if ( !ValidateDataFormat( DATA_FORMAT_STRING ) )
	{
		return;
	}

	uint32_t length;
	fread( &length, sizeof( uint32_t ), 1, filePtr );

	if ( length > bufLength )
	{
		length = bufLength;
	}

	// And now read in the string!
	memset( buffer, 0, bufLength );
	fread( buffer, sizeof( char ), length, filePtr );
}

PLVector2 Serializer::ReadCoordinate()
{
	if ( !ValidateDataFormat( DATA_FORMAT_COORDINATE ) )
	{
		return PLVector2();
	}

	PLVector2 vector;
	fread( &vector, sizeof( PLVector2 ), 1, filePtr );

	return vector;
}
