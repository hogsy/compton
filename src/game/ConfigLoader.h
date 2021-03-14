/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc {
	class ConfigLoader {
	public:
		explicit ConfigLoader( const char *path );
		~ConfigLoader();

		std::string GetIndex( unsigned int i ) {
			if ( i >= list_.size() ) {
				return "";
			}

			return list_[ i ];
		}

		unsigned int GetNumIndices() {
			return list_.size();
		}

	protected:
	private:
		std::vector< std::string > list_;
	};
}// namespace vc
