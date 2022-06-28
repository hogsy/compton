// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Compton.h"

#include "Action.h"
#include "InputManager.h"

using namespace ct;

bool input::Action::IsPressed()
{
	for ( auto i : keyboardBindings_ )
	{
		if ( inputManager->GetKeyState( i ) != State::PRESSED )
		{
			continue;
		}

		return true;
	}

	return false;
}
