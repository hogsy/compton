// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Serializer.h"

static const uint32_t storageVersion = 20200629;

ct::Serializer::Serializer( const char *path, ct::Serializer::Mode mode )
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

ct::Serializer::~Serializer()
{
	if ( filePtr != nullptr )
	{
		fclose( filePtr );
	}
}

bool ct::Serializer::ValidateDataFormat( uint8_t target )
{
	uint8_t format = fgetc( filePtr );
	if ( format != target )
	{
		Warning( "Invalid data format found, expected \"%d\" but got \"%d\"!\n", target, format );
		return false;
	}

	return true;
}

void ct::Serializer::WriteI8( int8_t var )
{
	fputc( DATA_FORMAT_I8, filePtr );
	fputc( var, filePtr );
}

void ct::Serializer::WriteI16( int16_t var )
{
	fputc( DATA_FORMAT_I16, filePtr );
	fwrite( &var, sizeof( int16_t ), 1, filePtr );
}

void ct::Serializer::WriteI32( int32_t var )
{
	fputc( DATA_FORMAT_I32, filePtr );
	fwrite( &var, sizeof( int32_t ), 1, filePtr );
}

void ct::Serializer::WriteF32( float var )
{
	fputc( DATA_FORMAT_FLOAT, filePtr );
	fwrite( &var, sizeof( float ), 1, filePtr );
}

void ct::Serializer::WriteString( const char *var )
{
	fputc( DATA_FORMAT_STRING, filePtr );

	// Write out the length
	uint32_t length = strlen( var );
	fwrite( &length, sizeof( uint32_t ), 1, filePtr );

	// And now write out the string itself
	fwrite( var, sizeof( char ), length, filePtr );
}

void ct::Serializer::WriteCoordinate( const PLVector2 &var )
{
	fputc( DATA_FORMAT_COORDINATE, filePtr );
	fwrite( &var, sizeof( PLVector2 ), 1, filePtr );
}

int8_t ct::Serializer::ReadI8()
{
	if ( !ValidateDataFormat( DATA_FORMAT_I8 ) )
		return 0;

	return ( int8_t ) fgetc( filePtr );
}

int16_t ct::Serializer::ReadI16()
{
	if ( !ValidateDataFormat( DATA_FORMAT_I16 ) )
		return 0;

	int16_t var;
	fread( &var, sizeof( int16_t ), 1, filePtr );
	return var;
}

int32_t ct::Serializer::ReadI32()
{
	if ( !ValidateDataFormat( DATA_FORMAT_I32 ) )
		return 0;

	int32_t var;
	fread( &var, sizeof( int32_t ), 1, filePtr );
	return var;
}

float ct::Serializer::ReadF32()
{
	if ( !ValidateDataFormat( DATA_FORMAT_FLOAT ) )
		return 0.0f;

	float var;
	fread( &var, sizeof( float ), 1, filePtr );
	return var;
}

void ct::Serializer::ReadString( char *buffer, size_t bufLength )
{
	if ( !ValidateDataFormat( DATA_FORMAT_STRING ) )
		return;

	uint32_t length;
	fread( &length, sizeof( uint32_t ), 1, filePtr );

	if ( length > bufLength )
		length = bufLength;

	// And now read in the string!
	memset( buffer, 0, bufLength );
	fread( buffer, sizeof( char ), length, filePtr );
}

PLVector2 ct::Serializer::ReadCoordinate()
{
	if ( !ValidateDataFormat( DATA_FORMAT_COORDINATE ) )
		return pl_vecOrigin2;

	PLVector2 vector;
	fread( &vector, sizeof( PLVector2 ), 1, filePtr );

	return vector;
}
