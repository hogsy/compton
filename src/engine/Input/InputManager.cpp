// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "InputManager.h"

using namespace vc;

input::Action *input::InputManager::PushAction( const char *description, ActionCallbackFunction callbackFunction )
{
	actions_.push_back( Action( description, callbackFunction ) );
	return &actions_[ actions_.size() - 1 ];
}
