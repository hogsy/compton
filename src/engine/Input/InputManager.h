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
			{
				return false;
			}

			return ( keys_[ key ] == State::PRESSED || keys_[ key ] == State::DOWN );
		}

		PUBLIC inline bool IsMouseButtonDown( int button ) const
		{
			if ( locked_ )
			{
				return false;
			}

			return ( mouseButtons_[ button ] == State::PRESSED || mouseButtons_[ button ] == State::DOWN );
		}

		PUBLIC bool HandleControllerEvent( unsigned int slot, int button, bool buttonUp );
		PUBLIC bool HandleKeyboardEvent( int key, bool keyUp );
		PUBLIC bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );

		// Called at the end of a frame; update the state from PRESSED to DOWN
		PUBLIC inline void EndFrame()
		{
			START_MEASURE();

			for ( unsigned int i = 0; i < keys_.size(); ++i )
			{
				if ( keys_[ i ] != State::PRESSED )
				{
					continue;
				}

				keys_[ i ] = State::DOWN;
			}

			for ( unsigned int i = 0; i < mouseButtons_.size(); ++i )
			{
				if ( mouseButtons_[ i ] != State::PRESSED )
				{
					continue;
				}

				mouseButtons_[ i ] = State::DOWN;
			}

			for ( unsigned int i = 0; i < controllers_.size(); ++i )
			{
				for ( unsigned int j = 0; j < Controller::MAX_CONTROLLER_BUTTONS; ++j )
				{
					if ( controllers_[ i ].buttonStates[ j ] != State::PRESSED )
					{
						continue;
					}

					controllers_[ i ].buttonStates[ j ] = State::DOWN;
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
		PRIVATE std::array< State, MAX_MOUSE_BUTTONS > mouseButtons_;

		PRIVATE std::array< State, ALLEGRO_KEY_MAX > keys_;

		// Actions
		PUBLIC Action *PushAction( const char *description );
		PRIVATE std::vector< Action > actions_;

		PUBLIC static constexpr unsigned int MAX_CONTROLLERS = 4;

		PRIVATE std::array< Controller, MAX_CONTROLLERS > controllers_;

		PRIVATE inline Controller *GetControllerForSlot( unsigned int slot )
		{
			if ( controllers_.empty() )
			{
				return nullptr;
			}

			// If the controller doesn't exist, fallback to the first
			if ( slot >= controllers_.size() )
			{
				slot = 0;
			}

			return &controllers_[ slot ];
		}

		PRIVATE bool locked_{ false };
	};

	extern InputManager *inputManager;
}// namespace ct::input
