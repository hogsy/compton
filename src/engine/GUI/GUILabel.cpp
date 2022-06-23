// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "GUILabel.h"

ct::GUILabel::GUILabel( ct::GUIPanel *parent, const char *label, int x, int y, int w, int h ) : GUIPanel( parent, x, y, w, h )
{
	SetText( label );
}

ct::GUILabel::~GUILabel()
{
}

void ct::GUILabel::Draw()
{
	SuperClass::Draw();

	bitmapFont_ = GetApp()->GetDefaultFont();
	if ( bitmapFont_ == nullptr )
		return;

	int cx, cy;
	GetParent()->GetContentPosition( &cx, &cy );

	//cx += x;
	//cy += y;

	bitmapFont_->DrawString( &cx, &cy, label_, hei::Colour( 0, 255, 255 ), true );
}

void ct::GUILabel::SetText( const char *label )
{
	snprintf( label_, sizeof( label_ ), "%s", label );
}

int ct::GUILabel::GetLength()
{
	int cw = bitmapFont_->GetCharacterWidth();
	int ch = bitmapFont_->GetCharacterHeight();
}
