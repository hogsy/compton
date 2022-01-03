// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace vc
{
	enum GUICursorMode
	{
		GUI_MOUSE_DEFAULT,
		GUI_MOUSE_DENY,
		GUI_MOUSE_MOVE,

		GUI_MOUSE_SIZER0,
		GUI_MOUSE_SIZER1,
		GUI_MOUSE_SIZER2,
		GUI_MOUSE_SIZER3,

		MAX_MOUSE_STATES
	};

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
