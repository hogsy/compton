/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2021, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <fstream>

#include "SimGame.h"
#include "ConfigLoader.h"

namespace vc {
	ConfigLoader::ConfigLoader( const char *path ) {
		std::ifstream fp( path );
		if ( !fp ) {
			Error( "Failed to open IDX file, \"%s\"!\n", path );
		}

		std::string line;
		while ( std::getline( fp, line ) ) {
			if ( line.empty() || line[ 0 ] == ';' ) {
				continue;
			}
			list_.push_back( line );
		}
	}

	ConfigLoader::~ConfigLoader() = default;
}// namespace vc
