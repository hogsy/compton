// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <allegro5/allegro.h>

namespace input
{
	enum class State : uint8_t
	{
		RELEASED,
		PRESSED,
		DOWN,
		MAX
	};

	enum MouseButton
	{
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,

		MAX_MOUSE_BUTTONS
	};

	struct Controller
	{
		static constexpr unsigned int MAX_CONTROLLER_BUTTONS = 16;
		State                         buttonStates[ MAX_CONTROLLER_BUTTONS ]{};

		static constexpr unsigned int MAX_CONTROLLER_AXIS = 6;
		PLVector2                     axisStates[ MAX_CONTROLLER_AXIS ]{};

		void *handle{};
	};

	class Action
	{
	public:
		inline explicit Action( const std::string &description ) : description_( description ) {}

		bool CheckStatus( State state ) const;

		[[nodiscard]] inline const char *GetDescription() const { return description_.c_str(); }

	private:
		std::string description_;

		std::vector< int > keyboardBindings;
		std::vector< int > buttonBindings_;

	public:
		inline void BindKey( int key )
		{
			// Check it's not been bound already
			auto i = std::find( keyboardBindings.begin(), keyboardBindings.end(), key );
			if ( i != keyboardBindings.end() )
				return;

			keyboardBindings.push_back( key );
		}
		inline void BindButton( int button )
		{
			// Check it's not been bound already
			auto i = std::find( buttonBindings_.begin(), buttonBindings_.end(), button );
			if ( i != buttonBindings_.end() )
				return;

			buttonBindings_.push_back( button );
		}
	};

	class InputManager
	{
	public:
		InputManager()  = default;
		~InputManager() = default;

		[[nodiscard]] inline State GetKeyState( int key ) const { return keys[ key ]; }
		[[nodiscard]] inline State GetMouseButtonState( int button ) const { return mouseButtons[ button ]; }

		[[nodiscard]] inline bool IsKeyDown( int key ) const
		{
			if ( locked )
				return false;

			return ( keys[ key ] == State::PRESSED || keys[ key ] == State::DOWN );
		}

		[[nodiscard]] inline bool IsMouseButtonDown( int button ) const
		{
			if ( locked )
				return false;

			return ( mouseButtons[ button ] == State::PRESSED || mouseButtons[ button ] == State::DOWN );
		}

		bool HandleControllerEvent( unsigned int slot, int button, bool buttonUp );
		bool HandleKeyboardEvent( int key, bool keyUp );
		bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );

		// Called at the end of a frame; update the state from PRESSED to DOWN
		inline void EndFrame()
		{
			for ( auto &key : keys )
			{
				if ( key != State::PRESSED )
					continue;

				key = State::DOWN;
			}

			for ( auto &mouseButton : mouseButtons )
			{
				if ( mouseButton != State::PRESSED )
					continue;

				mouseButton = State::DOWN;
			}

			for ( auto &controller : controllers_ )
			{
				for ( auto &buttonState : controller.buttonStates )
				{
					if ( buttonState != State::PRESSED )
						continue;

					buttonState = State::DOWN;
				}
			}

			locked = false;
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
		int   mx_{}, my_{}, mz_{};
		int   ox_{}, oy_{}, oz_{};
		int   dx_{}, dy_{}, dz_{};
		State mouseButtons[ MAX_MOUSE_BUTTONS ]{};

		State keys[ ALLEGRO_KEY_MAX ]{};

		// Actions
	public:
		Action *CreateAction( const char *description );

	private:
		std::map< std::string, Action > actions_;

	public:
		static constexpr unsigned int MAX_CONTROLLERS = 4;

	private:
		Controller controllers_[ MAX_CONTROLLERS ];

		inline Controller *GetControllerForSlot( unsigned int slot )
		{
			return &controllers_[ slot ];
		}

		bool locked{ false };
	};
}// namespace input
