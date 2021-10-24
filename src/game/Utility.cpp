/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "Utility.h"

/* https://overiq.com/c-examples/c-program-to-convert-a-decimal-number-to-roman-numerals/ */
const char *UTIL_GetRomanNumeralForNum( unsigned int num )
{
	static char buffer[ 16 ];
	memset( buffer, 0, sizeof( buffer ) );

	while ( num != 0 )
	{
		if ( num >= 1000 )
		{
			strcat( buffer, "M" );
			num -= 1000;
		}
		else if ( num >= 900 )
		{
			strcat( buffer, "CM" );
			num -= 900;
		}
		else if ( num >= 500 )
		{
			strcat( buffer, "D" );
			num -= 500;
		}
		else if ( num >= 400 )
		{
			strcat( buffer, "CD" );
			num -= 400;
		}
		else if ( num >= 100 )
		{
			strcat( buffer, "C" );
			num -= 100;
		}
		else if ( num >= 90 )
		{
			strcat( buffer, "XC" );
			num -= 90;
		}
		else if ( num >= 50 )
		{
			strcat( buffer, "L" );
			num -= 50;
		}
		else if ( num >= 40 )
		{
			strcat( buffer, "XL" );
			num -= 40;
		}
		else if ( num >= 10 )
		{
			strcat( buffer, "X" );
			num -= 10;
		}
		else if ( num >= 9 )
		{
			strcat( buffer, "IX" );
			num -= 9;
		}
		else if ( num >= 5 )
		{
			strcat( buffer, "V" );
			num -= 5;
		}
		else if ( num >= 4 )
		{
			strcat( buffer, "IV" );
			num -= 4;
		}
		else if ( num >= 1 )
		{
			strcat( buffer, "I" );
			num -= 1;
		}
	}

	return buffer;
}
