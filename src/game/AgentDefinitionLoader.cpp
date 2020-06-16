/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "AgentDefinitionLoader.h"
#include <sstream>

AgentDefinitionLoader::AgentDefinitionLoader() = default;
AgentDefinitionLoader::AgentDefinitionLoader( const std::string &path ) {
	std::ifstream fp( path );
	if ( !fp ) {
		PRINT_WARN( "Failed to open ADF file, \"%s\"!\n", path.c_str() );
		return;
	}

	std::string line;
	while ( std::getline( fp, line ) ) {
		std::istringstream iss( line );
		std::string a, b;
		if ( !( iss >> a >> b ) ) {
			break;
		}

		properties_.insert( std::pair< std::string, std::string >( a, b ) );
	}
}

std::string AgentDefinitionLoader::GetProperty( const std::string &property, const std::string &def ) const {
	auto p = properties_.find( property );
	if ( p == properties_.end() ) {
		PRINT_WARN( "Failed to find property \"%s\"!\n", property.c_str() );
		return def;
	}

	return p->second;
}
