// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include <random>
#include <cstring>

#include <plcore/pl.h>

#include "Random.h"

int vc::random::GenerateRandomInteger( int min, int max )
{
	static std::random_device       randomDevice;
	static std::mt19937             generator( randomDevice() );
	std::uniform_int_distribution<> distribution( min, max );
	return distribution( generator );
}

const char *vc::random::GenerateRandomName( char *buffer, size_t size )
{
	static const char *segments[] = {
			"aa",
			"al",
			"el",
			"la",
			"fa",
			"mo",
			"re",
			"ka",
			"ca",
			"ma",
			"fe",
			"me",
			"ra",
			"ke",
			"ce",
			"ee",
			"he",
			"fo",
			"ru",
			"ku",
			"cu",
			"eu",
			"hu",
			"fu",
	};

	unsigned int maxSize = GenerateRandomInteger( 4, size - 1 );
	char        *p       = buffer;
	for ( size_t i = 0; i < maxSize; i += 2 )
	{
		unsigned int s = GenerateRandomInteger( 0, PL_MAX_ARRAY_INDEX( segments ) );
		*p++           = segments[ s ][ 0 ];
		*p++           = segments[ s ][ 1 ];
	}

	// Ensure the first character is uppercase and null termination.
	buffer[ 0 ]       = std::toupper( buffer[ 0 ] );
	buffer[ maxSize ] = '\0';
	return buffer;
}
