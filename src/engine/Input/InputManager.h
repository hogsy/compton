// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <vector>
#include <algorithm>
#include <map>

#include <allegro5/keycodes.h>

#include "Input.h"
#include "Action.h"

namespace ct::input
{
	class InputManager
	{
		PUBLIC InputManager() = default;
		PUBLIC ~InputManager() = default;

		PUBLIC inline State GetKeyState( int key ) const { return keys_[ key ]; }
		PUBLIC inline State GetMouseButtonState( int button ) const { return mouseButtons_[ button ]; }

		PUBLIC inline bool IsKeyDown( int key ) const
		{
			if ( locked_ )
				return false;

			return ( keys_[ key ] == State::PRESSED || keys_[ key ] == State::DOWN );
		}

		PUBLIC inline bool IsMouseButtonDown( int button ) const
		{
			if ( locked_ )
				return false;

			return ( mouseButtons_[ button ] == State::PRESSED || mouseButtons_[ button ] == State::DOWN );
		}

		PUBLIC bool HandleControllerEvent( unsigned int slot, int button, bool buttonUp );
		PUBLIC bool HandleKeyboardEvent( int key, bool keyUp );
		PUBLIC bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );

		// Called at the end of a frame; update the state from PRESSED to DOWN
		PUBLIC inline void EndFrame()
		{
			START_MEASURE();

			for (auto & key : keys_)
			{
				if ( key != State::PRESSED )
					continue;

				key = State::DOWN;
			}

			for (auto & mouseButton : mouseButtons_)
			{
				if ( mouseButton != State::PRESSED )
					continue;

				mouseButton = State::DOWN;
			}

			for (auto & controller : controllers_)
			{
				for (auto & buttonState : controller.buttonStates)
				{
					if ( buttonState != State::PRESSED )
						continue;

					buttonState = State::DOWN;
				}
			}

			locked_ = false;

			END_MEASURE();
		}

		PUBLIC inline void GetMousePosition( int *x, int *y ) const
		{
			*x = mx_;
			*y = my_;
		}

		PUBLIC inline void GetMouseDelta( int *x, int *y ) const
		{
			*x = dx_;
			*y = dy_;
		}

		PUBLIC inline int GetMouseWheel() const
		{
			return mz_;
		}

		// Mouse coordinates, xy - z is wheel
		PRIVATE int mx_{ 0 }, my_{ 0 }, mz_{ 0 };
		PRIVATE int ox_{ 0 }, oy_{ 0 }, oz_{ 0 };
		PRIVATE int dx_{ 0 }, dy_{ 0 }, dz_{ 0 };
		PRIVATE State mouseButtons_[ MAX_MOUSE_BUTTONS ];

		PRIVATE State keys_[ ALLEGRO_KEY_MAX ];

		// Actions
		PUBLIC Action *PushAction( const char *description );
		PRIVATE std::vector< Action > actions_;

		PUBLIC static constexpr unsigned int MAX_CONTROLLERS = 4;
		PRIVATE Controller controllers_[ MAX_CONTROLLERS ];

		PRIVATE inline Controller *GetControllerForSlot( unsigned int slot )
		{
			assert( slot < MAX_CONTROLLERS );
			return &controllers_[ slot ];
		}

		PRIVATE bool locked_{ false };
	};

	extern InputManager *inputManager;
}// namespace ct::input
