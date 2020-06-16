/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <sstream>

#include "../shared.h"
#include "ConfigLoader.h"

namespace vc {
	ConfigLoader::ConfigLoader( const std::string &path ) {
		std::ifstream fp( path );
		if ( !fp ) {
			PRINT_ERROR( "Failed to open IDX file, \"%s\"!\n", path.c_str() );
		}

		std::string line;
		while ( std::getline( fp, line ) ) {
			list_.push_back( line );
		}
	}

	ConfigLoader::~ConfigLoader() = default;
}// namespace vc
