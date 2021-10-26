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

#include "GUIStyleSheet.h"
#include <vector>

struct ALLEGRO_BITMAP;

namespace vc
{
	class SpriteSheet;

	class GUIPanel
	{
	public:
		enum class Background
		{
			NONE,
			SOLID,
			TEXTURE,
		};
		enum class Border
		{
			NONE,
			INSET,
			OUTSET
		};

		GUIPanel( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 640, int h = 480, Background background = Background::NONE, Border border = Border::NONE );
		~GUIPanel();

		virtual void Draw();
		virtual void DrawBackground();

		virtual void Tick();

		void           SetStyleSheet( GUIStyleSheet *styleSheet );
		GUIStyleSheet *GetStyle() const { return myStyleSheet; }

		inline void     SetBackgroundColour( const PLColour &colour ) { backgroundColour = colour; }
		inline PLColour GetBackgroundColour() const { return backgroundColour; }

		inline void SetBorder( Border border ) { myBorder = border; }
		inline void SetBackground( Background background ) { myBackground = background; }

		inline GUIPanel *GetParent() const { return parentPtr; }

		inline void GetPosition( int *xd, int *yd ) const
		{
			*xd = x;
			*yd = y;
		}
		void        GetContentPosition( int *xd, int *yd ) const;
		inline void GetSize( int *wd, int *hd ) const
		{
			*wd = w;
			*hd = h;
		}
		void GetContentSize( int *wd, int *hd ) const;

		bool IsMouseOver( int mx, int my ) const;
		bool IsMouseOver() const;

		bool HandleMouseEvent( int mx, int my, int wheel, int button, bool buttonUp );
		bool HandleKeyboardEvent( int button, bool buttonUp );

		inline void SetVisible( bool flag )
		{
			isVisible = flag;
		}

		inline bool ShouldDraw()
		{
			if ( !isVisible )
			{
				return false;
			}
			else if ( x + w < 0 || x > 640 || y + h < 0 || y > 480 )
			{
				return false;
			}

			return true;
		}

	protected:
		GUIStyleSheet *myStyleSheet{ nullptr };

		std::vector< GUIPanel * > children;

		int x{ 0 }, y{ 0 };
		int w{ 640 }, h{ 480 };

		bool isDrawing{ false };// Flag on whether or not the panel is actually in view
		bool isVisible{ true }; // User flag, specifying if the panel should show or not

	private:
		void DrawBorder();
		void DrawBorderCorner( int dx, int dy, const RectangleCoord &tileCoord );
		void DrawBorderEdge( int dx, int dy, int dw, int dh, const RectangleCoord &tileCoord );

		Background myBackground{ Background::NONE };
		Border     myBorder{ Border::NONE };

		PLColour backgroundColour;

		GUIPanel *parentPtr{ nullptr };
	};
}// namespace vc
