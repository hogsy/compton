// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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

		int       ReadInteger();
		float     ReadFloat();
		void      ReadString( char *buffer, size_t bufLength );
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

	class ISerializable
	{
	public:
		virtual void Deserialize( Serializer *read ) = 0;
		virtual void Serialize( Serializer *write ) = 0;
	};
}// namespace vc
