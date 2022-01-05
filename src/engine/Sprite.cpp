// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#include "Sprite.h"

void vc::Sprite::Draw( int x, int y, bool alpha ) const
{
	DrawBitmap( pixels.data(), hasAlpha ? 4 : 3, x, y, width, height, alpha );
}
