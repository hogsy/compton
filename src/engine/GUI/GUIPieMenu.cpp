/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

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
