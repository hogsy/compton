// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace ct::input
{
	enum class State : uint8_t
	{
		RELEASED,
		PRESSED,
		DOWN,
		MAX
	};

	enum InputMouseButton
	{
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,

		MAX_MOUSE_BUTTONS
	};

	struct Controller
	{
		static constexpr unsigned int MAX_CONTROLLER_BUTTONS = 16;
		State buttonStates[ MAX_CONTROLLER_BUTTONS ];

		static constexpr unsigned int MAX_CONTROLLER_AXIS = 6;
		hei::Vector2 axisStates[ MAX_CONTROLLER_AXIS ];

		void *handle{ nullptr };
	};
}// namespace ct::input
