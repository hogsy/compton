/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#define MAX_SCRIPT_LENGTH   16384

namespace vc {
	class ScriptParser {
	public:
		ScriptParser( const char *path );
		~ScriptParser();

	protected:
		// Parsing
		const char *GetToken( char *buffer, size_t bufSize );

		void SkipSpaces();
		void SkipLine();

		unsigned int GetLineNumber() const { return lineNum; }
		unsigned int GetLinePosition() const { return linePos; }

		const char *AdvanceBufferPosition();

	private:
		char scriptBuffer[ MAX_SCRIPT_LENGTH ]{ '\0' };
		const char *bufPos{ nullptr };

		unsigned int lineNum{ 0 };
		unsigned int linePos{ 0 };
	};
}
