/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "GUIStyleSheet.h"
#include <vector>

struct ALLEGRO_BITMAP;

namespace vc {
	class SpriteSheet;

	class GUIPanel {
	public:
		enum class Background {
			NONE,
			SOLID,
			TEXTURE,
		};
		enum class Border {
			NONE,
			INSET,
			OUTSET
		};

		GUIPanel( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 640, int h = 480, Background background = Background::NONE, Border border = Border::NONE );
		~GUIPanel();

		virtual void Draw();
		virtual void DrawBackground();

		virtual void Tick();

		void SetStyleSheet( GUIStyleSheet *styleSheet );
		GUIStyleSheet *GetStyle() const { return myStyleSheet; }

		PL_INLINE void SetBackgroundColour( const PLColour &colour )    { backgroundColour = colour; }
		PL_INLINE PLColour GetBackgroundColour() const                  { return backgroundColour; }

		PL_INLINE void SetBorder( Border border ) { myBorder = border; }
		PL_INLINE void SetBackground( Background background ) { myBackground = background; }

		PL_INLINE GUIPanel *GetParent() const { return parentPtr; }

		PL_INLINE void GetPosition( int *xd, int *yd ) const { *xd = x; *yd = y; }
		void GetContentPosition( int *xd, int *yd ) const;
		PL_INLINE void GetSize( int *wd, int *hd ) const { *wd = w; *hd = h; }
		void GetContentSize( int *wd, int *hd ) const;

		bool IsMouseOver() const;

	protected:
		GUIStyleSheet *myStyleSheet{ nullptr };

		std::vector< GUIPanel * > children;

		int x{ 0 }, y{ 0 };
		int w{ 640 }, h{ 480 };

		bool shouldDraw{ false };

	private:
		void DrawBorder();
		void DrawBorderCorner( int dx, int dy, const RectangleCoord &tileCoord );
		void DrawBorderEdge( int dx, int dy, int dw, int dh, const RectangleCoord &tileCoord );

		Background myBackground{ Background::NONE };
		Border myBorder{ Border::NONE };

		PLColour backgroundColour;

		GUIPanel *parentPtr{ nullptr };
	};
}
