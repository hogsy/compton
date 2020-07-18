/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include <PL/platform_filesystem.h>

#include "ScriptParser.h"
#include "SimGame.h"

vc::ScriptParser::ScriptParser( const char *path ) {
	Print( "Loading script, \"%s\"\n", path );

	PLFile *filePtr = plOpenFile( path, true );
	if ( filePtr == nullptr ) {
		Error( "Failed to open script, \"%s\"!\nPL: %s\n", path, plGetError() );
	}

	size_t length = plGetFileSize( filePtr );
	if ( length == 0 ) {
		Warning( "Specified script \"%s\" is empty!\n", path );
		plCloseFile( filePtr );
		return;
	}

	if ( length >= MAX_SCRIPT_LENGTH ) {
		Warning( "Script \"%s\" is too long (%d vs %d), some lazy plonker decided to have a fixed buffer size!\n", path, length, MAX_SCRIPT_LENGTH );
		length = MAX_SCRIPT_LENGTH;
	}

	const uint8_t *fileDataPtr = plGetFileData( filePtr );
	memcpy( scriptBuffer, fileDataPtr, length );

	// Fetch the starting position
	bufPos = scriptBuffer;
	if ( *bufPos == '\0' ) {
		Warning( "Specified script \"%s\" is empty!\n", path );
	}

	plCloseFile( filePtr );
}

vc::ScriptParser::~ScriptParser() = default;

/**
 * Reads in a token from the script.
 */
const char *vc::ScriptParser::GetToken( char *buffer, size_t bufSize ) {
	bool isContained = false;
	if ( *bufPos == '"' ) {
		isContained = true;
		AdvanceBufferPosition();
	}

	unsigned int i;
	for ( i = 0; i < bufSize; ++i ) {
		// Stop if we either hit the end or a new line
		if ( bufPos[ 0 ] == '\0' || bufPos[ 0 ] == '\r' || bufPos[ 0 ] == '\n' ) {
			break;
		}

		// Quotations are handled in their own special way
		if ( ( isContained && *bufPos == '"' ) || (!isContained && *bufPos == ' ' ) ) {
			AdvanceBufferPosition();
			break;
		}

		buffer[ i ] = *AdvanceBufferPosition();
	}

	// Ensure the returned string is null-terminated
	buffer[ i ] = '\0';

	// If it's a new line
	if ( bufPos[ 0 ] == '\r' || bufPos[ 1 ] == '\n' ) {
		SkipLine();
	}

	if ( IsEndOfFile() ) {
		return nullptr;
	}

	// Otherwise return the string
	return buffer;
}

bool vc::ScriptParser::IsEndOfFile() {
	return *bufPos == '\0';
}

void vc::ScriptParser::SkipSpaces() {
	while( *bufPos == ' ' ) {
		AdvanceBufferPosition();
	}
}

void vc::ScriptParser::SkipLine() {
	while( *bufPos != '\0' ) {
		if ( bufPos[ 0 ] == '\r' || bufPos[ 0 ] == '\n' ) {
			break;
		}

		AdvanceBufferPosition();
	}

	if ( bufPos[ 0 ] == '\r' ) {
		AdvanceBufferPosition();
	}

	if ( bufPos[ 0 ] == '\n' ) {
		AdvanceBufferPosition();
	}

	lineNum++;
	linePos = 0;
}

/**
 * Increment and return the current internal buffer position.
 */
const char *vc::ScriptParser::AdvanceBufferPosition() {
	linePos++;
	return bufPos++;
}
