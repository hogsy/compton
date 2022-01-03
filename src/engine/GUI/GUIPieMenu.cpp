// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
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
