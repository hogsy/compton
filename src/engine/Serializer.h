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

#pragma once

#include "../shared.h"

namespace vc
{
	class Serializer
	{
	public:
		enum class Mode
		{
			READ,
			WRITE,
		};

		Serializer( const char *path, Mode mode );
		~Serializer();

		bool ValidateDataFormat( uint8_t target );

		void WriteInteger( int var );
		void WriteFloat( float var );
		void WriteString( const char *var );
		void WriteCoordinate( const PLVector2 &var );

		int		  ReadInteger();
		float	  ReadFloat();
		void	  ReadString( char *buffer, size_t bufLength );
		PLVector2 ReadCoordinate();

		inline unsigned int GetVersion() const { return version; }

	private:
		enum : uint8_t
		{
			DATA_FORMAT_STRING,
			DATA_FORMAT_COORDINATE,
			DATA_FORMAT_INTEGER,
			DATA_FORMAT_FLOAT,
		};

		unsigned int version{ 0 };

		FILE *filePtr{ nullptr };
	};
}// namespace vc
