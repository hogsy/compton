// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace vc
{
	class GUICursor : public GUIPanel
	{
	public:
		GUICursor( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 32, int h = 32 );
		~GUICursor();

		void DrawBackground() override;

		void Tick() override;

	protected:
	private:
		GUICursorMode currentCursorMode{ GUICursorMode::GUI_MOUSE_DEFAULT };
	};
}// namespace vc
