/*
SimGame Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SimGame.h"
#include "GUIPieMenu.h"

vc::GUIPieMenu::GUIPieMenu( vc::GUIPanel *parent ) : GUIPanel( parent )
{
}

vc::GUIPieMenu::~GUIPieMenu()
{
}

/**
 * Push a new item onto the radio button list.
 */
void vc::GUIPieMenu::PushOption( const char *label, vc::GUIPieMenu::OptionCallback callback )
{
}
