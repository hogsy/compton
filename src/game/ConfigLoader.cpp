/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <fstream>

#include "../shared.h"
#include "ConfigLoader.h"

namespace vc {
	ConfigLoader::ConfigLoader( const char *path ) {
		std::ifstream fp( path );
		if ( !fp ) {
			Error( "Failed to open IDX file, \"%s\"!\n", path );
		}

		std::string line;
		while ( std::getline( fp, line ) ) {
			list_.push_back( line );
		}
	}

	ConfigLoader::~ConfigLoader() = default;
}// namespace vc
