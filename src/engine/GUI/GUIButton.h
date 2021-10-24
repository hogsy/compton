/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "GUIPanel.h"

namespace vc
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

		char myLabel[ 64 ];
	};
}// namespace vc
