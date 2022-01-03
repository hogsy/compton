// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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

int vc::random::PerlinNoise::permutation[] = {
		151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };

vc::random::PerlinNoise::PerlinNoise()
{
	for ( unsigned int i = 0; i < 256; ++i )
	{
		p[ 256 + i ] = p[ i ] = permutation[ i ];
	}
}

vc::random::PerlinNoise::PerlinNoise( int seed )
{
	srand( seed );
	for ( unsigned int i = 0; i < 256; ++i )
	{
		p[ 256 + i ] = p[ i ] = rand() % 256;
	}
}

/**
 * Perlin noise generation, based on 'Improved Noise'.
 * https://mrl.nyu.edu/~perlin/noise/
 */
double vc::random::PerlinNoise::Noise( double x, double y, double z )
{
	// Find unit cube that contains point
	int X = ( int ) floor( x ) & 255;
	int Y = ( int ) floor( y ) & 255;
	int Z = ( int ) floor( z ) & 255;

	// Find relative X, Y and Z of point in cube
	x -= floor( x );
	y -= floor( y );
	z -= floor( z );

	// Compute fade curves for each of X, Y and Z
	double u = Fade( x );
	double v = Fade( y );
	double w = Fade( z );

	// Hash coordinates of the 8 cube corners
	int A = p[ X ] + Y, AA = p[ A ] + Z, AB = p[ A + 1 ] + Z,
		B = p[ X + 1 ] + Y, BA = p[ B ] + Z, BB = p[ B + 1 ] + Z;

	// And add blended results from 8 corners of cube
	return Lerp( w, Lerp( v, Lerp( u, Grad( p[ AA ], x, y, z ), Grad( p[ BA ], x - 1, y, z ) ), Lerp( u, Grad( p[ AB ], x, y - 1, z ), Grad( p[ BB ], x - 1, y - 1, z ) ) ),
	             Lerp( v, Lerp( u, Grad( p[ AA + 1 ], x, y, z - 1 ), Grad( p[ BA + 1 ], x - 1, y, z - 1 ) ),
	                   Lerp( u, Grad( p[ AB + 1 ], x, y - 1, z - 1 ),
	                         Grad( p[ BB + 1 ], x - 1, y - 1, z - 1 ) ) ) );
}

double vc::random::PerlinNoise::Fade( double t )
{
	return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
}

double vc::random::PerlinNoise::Lerp( double t, double a, double b )
{
	return a + t * ( b - a );
}

double vc::random::PerlinNoise::Grad( int hash, double x, double y, double z )
{
	// Convert lo 4 bits of hash code into 12 gradient directions
	int    h = hash & 15;
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x
	                                          : z;
	return ( ( h & 1 ) == 0 ? u : -u ) + ( ( h & 2 ) == 0 ? v : -v );
}
