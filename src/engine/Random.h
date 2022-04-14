// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace ct
{
	namespace random
	{
		int         GenerateRandomInteger( int min, int max );
		const char *GenerateRandomName( char *buffer, size_t size );

		class PerlinNoise
		{
		public:
			PerlinNoise();
			PerlinNoise( int seed );

			double Noise( double x, double y, double z );

		private:
			double Fade( double t );
			double Lerp( double t, double a, double b );
			double Grad( int hash, double x, double y, double z );

			int        p[ 512 ];
			static int permutation[];
		};
	}// namespace random
}// namespace vc
