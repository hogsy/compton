// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

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
		std::string        a, b;
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
