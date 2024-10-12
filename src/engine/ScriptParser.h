// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#define MAX_SCRIPT_LENGTH 16384

namespace vc
{
	class ScriptParser
	{
	public:
		ScriptParser( const char *path );
		~ScriptParser();

	protected:
		// Parsing
		const char *GetToken( char *buffer, size_t bufSize );

		bool IsEndOfFile();

		void SkipSpaces();
		void SkipLine();

		unsigned int GetLineNumber() const { return lineNum; }
		unsigned int GetLinePosition() const { return linePos; }

		const char *AdvanceBufferPosition();

	private:
		char        scriptBuffer[ MAX_SCRIPT_LENGTH ]{ '\0' };
		const char *bufPos{ nullptr };

		unsigned int lineNum{ 0 };
		unsigned int linePos{ 0 };
	};
}// namespace vc
