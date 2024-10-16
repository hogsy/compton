// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"

#include "input.h"

bool input::Action::CheckStatus( input::State state ) const
{
	for ( auto i : keyboardBindings )
	{
		if ( manager->GetKeyState( i ) != state )
		{
			continue;
		}

		return true;
	}

	return false;
}
