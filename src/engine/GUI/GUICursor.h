/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "GUIPanel.h"

namespace vc {
	class GUICursor : public GUIPanel {
	public:
		GUICursor( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 32, int h = 32 );
		~GUICursor();

		void DrawBackground() override;

		void Tick() override;

	protected:
	private:
		GUICursorMode currentCursorMode{ GUICursorMode::GUI_MOUSE_DEFAULT };
	};
}
