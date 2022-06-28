// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <cstdint>
#include <string>

#include <plcore/pl_math.h>

namespace ct
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

		void WriteI8( int8_t var );
		void WriteI16( int16_t var );
		void WriteI32( int32_t var );
		void WriteF32( float var );
		void WriteString( const char *var );
		void WriteCoordinate( const PLVector2 &var );

		int8_t ReadI8();
		int16_t ReadI16();
		int32_t ReadI32();
		float ReadF32();
		std::string ReadString();
		PLVector2 ReadCoordinate();

		inline unsigned int GetVersion() const { return version; }

	private:
		enum : uint8_t
		{
			DATA_FORMAT_STRING,
			DATA_FORMAT_COORDINATE,
			DATA_FORMAT_I8,
			DATA_FORMAT_I16,
			DATA_FORMAT_I32,
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
}// namespace ct
