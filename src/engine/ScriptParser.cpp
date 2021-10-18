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

#include <plcore/pl_filesystem.h>

#include "ScriptParser.h"
#include "SimGame.h"

vc::ScriptParser::ScriptParser( const char *path )
{
	Print( "Loading script, \"%s\"\n", path );

	PLFile *filePtr = PlOpenFile( path, true );
	if ( filePtr == nullptr )
	{
		Error( "Failed to open script, \"%s\"!\nPL: %s\n", path, PlGetError() );
	}

	size_t length = PlGetFileSize( filePtr );
	if ( length == 0 )
	{
		Warning( "Specified script \"%s\" is empty!\n", path );
		PlCloseFile( filePtr );
		return;
	}

	if ( length >= MAX_SCRIPT_LENGTH )
	{
		Warning( "Script \"%s\" is too long (%d vs %d), some lazy plonker decided to have a fixed buffer size!\n", path, length, MAX_SCRIPT_LENGTH );
		length = MAX_SCRIPT_LENGTH;
	}

	const uint8_t *fileDataPtr = PlGetFileData( filePtr );
	memcpy( scriptBuffer, fileDataPtr, length );

	// Fetch the starting position
	bufPos = scriptBuffer;
	if ( *bufPos == '\0' )
	{
		Warning( "Specified script \"%s\" is empty!\n", path );
	}

	PlCloseFile( filePtr );
}

vc::ScriptParser::~ScriptParser() = default;

/**
 * Reads in a token from the script.
 */
const char *vc::ScriptParser::GetToken( char *buffer, size_t bufSize )
{
	bool isContained = false;
	if ( *bufPos == '"' )
	{
		isContained = true;
		AdvanceBufferPosition();
	}

	// If it's a new line
	if ( bufPos[ 0 ] == '\r' || bufPos[ 1 ] == '\n' )
	{
		SkipLine();
	}

	unsigned int i;
	for ( i = 0; i < bufSize; ++i )
	{
		// Stop if we either hit the end or a new line
		if ( bufPos[ 0 ] == '\0' || bufPos[ 0 ] == '\r' || bufPos[ 0 ] == '\n' )
		{
			break;
		}

		// Quotations are handled in their own special way
		if ( ( isContained && *bufPos == '"' ) || ( !isContained && *bufPos == ' ' ) )
		{
			AdvanceBufferPosition();
			break;
		}

		buffer[ i ] = *AdvanceBufferPosition();
	}

	// Ensure the returned string is null-terminated
	buffer[ i ] = '\0';

	// If it's a new line
	if ( bufPos[ 0 ] == '\r' || bufPos[ 1 ] == '\n' )
	{
		SkipLine();
	}

	if ( IsEndOfFile() )
	{
		return nullptr;
	}

	// Otherwise return the string
	return buffer;
}

bool vc::ScriptParser::IsEndOfFile()
{
	return *bufPos == '\0';
}

void vc::ScriptParser::SkipSpaces()
{
	while ( *bufPos == ' ' )
	{
		AdvanceBufferPosition();
	}
}

void vc::ScriptParser::SkipLine()
{
	while ( *bufPos != '\0' )
	{
		if ( bufPos[ 0 ] == '\r' || bufPos[ 0 ] == '\n' )
		{
			break;
		}

		AdvanceBufferPosition();
	}

	if ( bufPos[ 0 ] == '\r' )
	{
		AdvanceBufferPosition();
	}

	if ( bufPos[ 0 ] == '\n' )
	{
		AdvanceBufferPosition();
	}

	lineNum++;
	linePos = 0;
}

/**
 * Increment and return the current internal buffer position.
 */
const char *vc::ScriptParser::AdvanceBufferPosition()
{
	linePos++;
	return bufPos++;
}
