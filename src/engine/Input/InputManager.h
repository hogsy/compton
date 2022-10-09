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
	public:
		InputManager() = default;
		~InputManager() = default;

		[[nodiscard]] inline State GetKeyState( int key ) const { return keys_[ key ]; }
		[[nodiscard]] inline State GetMouseButtonState( int button ) const { return mouseButtons_[ button ]; }

		[[nodiscard]] inline bool IsKeyDown( int key ) const
		{
			if ( locked_ )
				return false;

			return ( keys_[ key ] == State::PRESSED || keys_[ key ] == State::DOWN );
		}

		[[nodiscard]] inline bool IsMouseButtonDown( int button ) const
		{
			if ( locked_ )
				return false;

			return ( mouseButtons_[ button ] == State::PRESSED || mouseButtons_[ button ] == State::DOWN );
		}

		bool HandleControllerEvent( unsigned int slot, int button, bool buttonUp );
		bool HandleKeyboardEvent( int key, bool keyUp );
		bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );

		// Called at the end of a frame; update the state from PRESSED to DOWN
		inline void EndFrame()
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

		inline void GetMousePosition( int *x, int *y ) const
		{
			*x = mx_;
			*y = my_;
		}

		inline void GetMouseDelta( int *x, int *y ) const
		{
			*x = dx_;
			*y = dy_;
		}

		inline int GetMouseWheel() const
		{
			return mz_;
		}

	private:
		// Mouse coordinates, xy - z is wheel
		int mx_{ 0 }, my_{ 0 }, mz_{ 0 };
		int ox_{ 0 }, oy_{ 0 }, oz_{ 0 };
		int dx_{ 0 }, dy_{ 0 }, dz_{ 0 };
		State mouseButtons_[ MAX_MOUSE_BUTTONS ];

		State keys_[ ALLEGRO_KEY_MAX ];

		// Actions
	public:
		Action *PushAction( const char *description );
	private:
		std::vector< Action > actions_;

	public:
		static constexpr unsigned int MAX_CONTROLLERS = 4;
	private:
		Controller controllers_[ MAX_CONTROLLERS ];

		inline Controller *GetControllerForSlot( unsigned int slot )
		{
			assert( slot < MAX_CONTROLLERS );
			return &controllers_[ slot ];
		}

		bool locked_{ false };
	};

	extern InputManager *inputManager;
}// namespace ct::input
