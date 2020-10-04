/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <random>

#include "Random.h"

int vc::random::GenerateRandomInteger( int min, int max ) {
	std::random_device randomDevice;
	std::mt19937 generator( randomDevice() );
	std::uniform_int_distribution<> distribution( min, max );
	return distribution( generator );
}
