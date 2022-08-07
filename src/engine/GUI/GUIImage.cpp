// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Compton.h"

#include "GUIImage.h"

ct::GUIImage::GUIImage( ct::GUIPanel *parent, const ct::Sprite *sprite, int x, int y, int w, int h )
: GUIPanel( parent, x, y, w, h, Background::NONE, Border::NONE )
{
	sprite_ = sprite;
}

ct::GUIImage::~GUIImage()
{
}

void ct::GUIImage::DrawBackground()
{
	SuperClass::DrawBackground();

	if ( sprite_ == nullptr )
		return;

	int px, py;
	GetPosition( &px, &py );
	sprite_->Draw( px, py );
}
