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

#include "AgentDefinitionLoader.h"
#include <sstream>

AgentDefinitionLoader::AgentDefinitionLoader() = default;
AgentDefinitionLoader::AgentDefinitionLoader( const std::string &path )
{
	std::ifstream fp( path );
	if ( !fp )
	{
		Warning( "Failed to open ADF file, \"%s\"!\n", path.c_str() );
		return;
	}

	std::string line;
	while ( std::getline( fp, line ) )
	{
		std::istringstream iss( line );
		std::string		   a, b;
		if ( !( iss >> a >> b ) )
		{
			break;
		}

		properties_.insert( std::pair< std::string, std::string >( a, b ) );
	}
}

std::string AgentDefinitionLoader::GetProperty( const std::string &property, const std::string &def ) const
{
	auto p = properties_.find( property );
	if ( p == properties_.end() )
	{
		Warning( "Failed to find property \"%s\"!\n", property.c_str() );
		return def;
	}

	return p->second;
}
