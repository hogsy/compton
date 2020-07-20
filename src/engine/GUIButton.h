/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "GUIPanel.h"

namespace vc {
	class GUIButton : public GUIPanel {
	public:
		GUIButton( GUIPanel *parent = nullptr, const char *label = nullptr, int x = 0, int y = 0, int w = 32, int h = 16 );
		~GUIButton();

	protected:
	private:
		void ( *ButtonCallback )( void *userDataPtr );

		char myLabel[ 64 ];
	};
}// namespace vc
