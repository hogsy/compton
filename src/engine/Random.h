/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc
{
	namespace random
	{
		int			GenerateRandomInteger( int min, int max );
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

			int		   p[ 512 ];
			static int permutation[];
		};
	}// namespace random
}// namespace vc
