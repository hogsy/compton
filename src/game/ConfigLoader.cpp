/*
SimGame Engine
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

#include <fstream>

#include "SimGame.h"
#include "ConfigLoader.h"

namespace vc
{
	ConfigLoader::ConfigLoader( const char *path )
	{
		std::ifstream fp( path );
		if ( !fp )
		{
			Error( "Failed to open IDX file, \"%s\"!\n", path );
		}

		std::string line;
		while ( std::getline( fp, line ) )
		{
			if ( line.empty() || line[ 0 ] == ';' )
			{
				continue;
			}
			list_.push_back( line );
		}
	}

	ConfigLoader::~ConfigLoader() = default;
}// namespace vc
