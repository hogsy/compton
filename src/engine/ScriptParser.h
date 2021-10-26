/*
Compton, 2D Game Engine
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
