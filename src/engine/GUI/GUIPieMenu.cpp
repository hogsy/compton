// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GUIPieMenu.h"

ct::GUIPieMenu::GUIPieMenu( ct::GUIPanel *parent ) : GUIPanel( parent )
{
}

ct::GUIPieMenu::~GUIPieMenu()
{
}

/**
 * Push a new item onto the radio button list.
 */
void ct::GUIPieMenu::PushOption( const char *label, ct::GUIPieMenu::OptionCallback callback )
{
}
