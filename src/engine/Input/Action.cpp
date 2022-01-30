// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Action.h"

using namespace vc;

input::Action::Action( const std::string &description, ActionCallbackFunction callbackFunction )
	: description_( description ), callbackFunction_( callbackFunction )
{
}
