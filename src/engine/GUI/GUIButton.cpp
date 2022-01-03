// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GUIButton.h"

#include "../shared.h"

vc::GUIButton::GUIButton( vc::GUIPanel *parent, const char *label, int x, int y, int w, int h )
	: GUIPanel( parent, x, y, w, h, Background::TEXTURE, Border::OUTSET )
{
	strncpy( myLabel, label, sizeof( myLabel ) );
}

vc::GUIButton::~GUIButton()
{
}

void vc::GUIButton::Tick()
{
	SuperClass::Tick();

	if ( !IsMouseOver() )
	{
		SetBorder( Border::OUTSET );
		return;
	}

	SetBorder( Border::INSET );
}
