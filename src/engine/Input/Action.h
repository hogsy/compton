// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <vector>
#include <algorithm>
#include <string>

#include "Input.h"

namespace vc::input
{
	typedef void ( *ActionCallbackFunction )();
	typedef unsigned int ActionSlot;

	class Action
	{
	public:
		Action( const std::string &description, ActionCallbackFunction callbackFunction );

		inline const char *GetDescription() const { return description_.c_str(); }

	private:
		std::string description_;
		ActionCallbackFunction callbackFunction_;

		std::vector< Key > keyboardBindings_;
		std::vector< Button > buttonBindings_;

	public:
		inline void BindKey( Key key )
		{
			// Check it's not been bound already
			auto i = std::find( keyboardBindings_.begin(), keyboardBindings_.end(), key );
			if ( i != keyboardBindings_.end() )
				return;

			keyboardBindings_.push_back( key );
		}
		inline void BindButton( Button button )
		{
			// Check it's not been bound already
			auto i = std::find( buttonBindings_.begin(), buttonBindings_.end(), button );
			if ( i != buttonBindings_.end() )
				return;

			buttonBindings_.push_back( button );
		}
	};
}// namespace vc::input
