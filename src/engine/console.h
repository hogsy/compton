// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace engine
{
	class Console
	{
	public:
		Console();
		~Console();

		inline bool IsOpen() const { return isOpen; }

		void Open();
		void Close();

		void Draw();

		void PushCharacter( char c );

		void ScrollHistory( bool forward );

	private:
		bool isOpen{};

		std::string                buffer;
		std::vector< std::string > history;
		unsigned int               historyPosition{};
	};
}// namespace engine
