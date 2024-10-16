// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"

#include "GUIButton.h"

GUIButton::GUIButton( GUIPanel *parent, const char *label, int x, int y, int w, int h )
    : GUIPanel( parent, x, y, w, h, Background::TEXTURE, Border::OUTSET )
{
	strncpy( myLabel, label, sizeof( myLabel ) );
}

GUIButton::~GUIButton()
{
}

void GUIButton::Tick()
{
	SuperClass::Tick();

	if ( !IsMouseOver() )
	{
		SetBorder( Border::OUTSET );
		return;
	}

	SetBorder( Border::INSET );
}
