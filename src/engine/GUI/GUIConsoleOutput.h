/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "GUIPanel.h"

namespace vc {
	class GUIConsoleOutput : public GUIPanel {
	public:
		GUIConsoleOutput( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 32, int h = 32 );
		~GUIConsoleOutput();

		void DrawBackground() override;

		void Tick() override;

	protected:
	private:
		std::vector< char[ 256 ] > consoleLines;
	};
}
