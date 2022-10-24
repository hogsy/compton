// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

#include "../Renderer/BitmapFont.h"

namespace ct
{
	class GUILabel : public GUIPanel
	{
	public:
		IMPLEMENT_SUPER( GUIPanel )

		GUILabel( GUIPanel *parent = nullptr, const char *label = nullptr, int x = 0, int y = 0, int w = 64, int h = 16 );
		~GUILabel();

		void Draw();

		void SetText( const char *label );

	private:
		char label_[ 256 ];

		BitmapFont *bitmapFont_;
	};
}// namespace ct
