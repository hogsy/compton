// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <vector>
#include <algorithm>

#include <plcore/pl_math_vector.h>

#include "Input.h"
#include "Action.h"

namespace vc::input
{
	class InputManager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		Action *PushAction( const char *description, ActionCallbackFunction callbackFunction );

		bool IsActionPressed( ActionSlot actionSlot ) const;

	private:
		std::vector< Action > actions_;
		std::map< Button, ActionSlot > buttonToActionSlot_;

		struct Controller
		{
			Controller( unsigned int slot );

			static constexpr unsigned int MAX_CONTROLLER_BUTTONS = 16;
			bool buttonStates[ MAX_CONTROLLER_BUTTONS ];

			static constexpr unsigned int MAX_CONTROLLER_AXIS = 6;
			hei::Vector2 axisStates[ MAX_CONTROLLER_AXIS ];

			void *handle;
		};

		std::vector< Controller > controllers_;

		inline Controller *GetControllerForSlot( unsigned int slot )
		{
			if ( controllers_.empty() )
				return nullptr;

			// If the controller doesn't exist, fallback to the first
			if ( slot >= controllers_.size() )
				slot = 0;

			return &controllers_[ slot ];
		}
	};

	extern InputManager *inputManager;
}// namespace vc::input
