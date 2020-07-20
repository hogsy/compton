/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc {
	class StateStorage {
	public:
		enum class Mode {
			READ,
			WRITE,
		};

		StateStorage( const char *path, Mode mode );
		~StateStorage();

		bool ValidateDataFormat( uint8_t target );

		void WriteString( const char *var );
		void WriteCoordinate( const PLVector2 &var );

		void ReadString( char *buffer, size_t bufLength );
		PLVector2 ReadCoordinate();

	private:
		enum : uint8_t {
			DATA_FORMAT_STRING,
			DATA_FORMAT_COORDINATE,
		};

		unsigned int version{ 0 };

		FILE *filePtr{ nullptr };
	};
}
