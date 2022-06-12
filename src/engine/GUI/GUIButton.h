// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace ct
{
	class GUIButton : public GUIPanel
	{
	public:
		IMPLEMENT_SUPER( GUIPanel )

		GUIButton( GUIPanel *parent = nullptr, const char *label = nullptr, int x = 0, int y = 0, int w = 32, int h = 16 );
		~GUIButton();

		void Tick() override;

	protected:
	private:
		void ( *ButtonCallback )( void *userDataPtr );

		char label_[ 64 ];
	};
}// namespace vc
