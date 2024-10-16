// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"

#include "GUIPieMenu.h"

GUIPieMenu::GUIPieMenu( GUIPanel *parent ) : GUIPanel( parent )
{
}

GUIPieMenu::~GUIPieMenu()
{
}

/**
 * Push a new item onto the radio button list.
 */
void GUIPieMenu::PushOption( const char *label, GUIPieMenu::OptionCallback callback )
{
}
