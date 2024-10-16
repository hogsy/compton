// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

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
