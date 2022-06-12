// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GUIButton.h"

ct::GUIButton::GUIButton( ct::GUIPanel *parent, const char *label, int x, int y, int w, int h )
	: GUIPanel( parent, x, y, w, h, Background::DEFAULT, Border::OUTSET )
{
	strncpy( label_, label, sizeof( label_ ) );
}

ct::GUIButton::~GUIButton()
{
}

void ct::GUIButton::Tick()
{
	SuperClass::Tick();

	if ( !IsMouseOver() )
	{
		SetBorder( Border::OUTSET );
		return;
	}

	SetBorder( Border::INSET );
}
