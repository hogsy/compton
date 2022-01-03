// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace vc
{
	class GUIConsoleOutput : public GUIPanel
	{
	public:
		GUIConsoleOutput( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 32, int h = 32 );
		~GUIConsoleOutput();

		void DrawBackground() override;

		void Tick() override;

	protected:
	private:
		std::vector< char[ 256 ] > consoleLines;
	};
}// namespace vc
