// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace ct
{
	class Console
	{
	public:
		Console();
		~Console();

		inline bool IsOpen() const { return isOpen_; }

		void Open();
		void Close();

		void Draw();

		void PushCharacter( int c );

		void ScrollHistory( bool forward );

	private:
		bool isOpen_{ false };

		std::string buffer_;
		std::vector< std::string > history_;
		unsigned int historyPosition_{ 0 };
	};
}