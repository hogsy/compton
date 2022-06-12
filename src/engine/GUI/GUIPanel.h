// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <vector>

struct ALLEGRO_BITMAP;

namespace ct
{
	class SpriteSheet;
	class GUIStyleSheet;

	class GUIPanel
	{
	public:
		enum class Background
		{
			NONE,
			DEFAULT,
			SOLID,
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

		// Below can be overridden to introduce animations etc.
		virtual void Show()
		{
			SetVisible( true );
		}
		virtual void Hide()
		{
			SetVisible( false );
		}

		virtual void Tick();

		void SetStyleSheet( GUIStyleSheet *styleSheet );
		GUIStyleSheet *GetStyle() const { return styleSheet_; }

		inline void SetBackgroundColour( const hei::Colour &colour ) { backgroundColour_ = colour; }
		inline hei::Colour GetBackgroundColour() const { return backgroundColour_; }

		inline void SetBorder( Border border ) { border_ = border; }
		inline void SetBackground( Background background ) { background_ = background; }

		inline GUIPanel *GetParent() const { return parentPtr; }

		inline void GetPosition( int *xd, int *yd ) const
		{
			*xd = x;
			*yd = y;
		}
		void GetContentPosition( int *xd, int *yd ) const;
		inline void GetSize( int *wd, int *hd ) const
		{
			*wd = w;
			*hd = h;
		}
		void GetContentSize( int *wd, int *hd ) const;

		[[nodiscard]] bool IsMouseOver( int mx, int my ) const;
		[[nodiscard]] bool IsMouseOver() const;

		bool HandleMouseEvent( int mx, int my, int wheel, int button, bool buttonUp );
		bool HandleKeyboardEvent( int button, bool buttonUp );

		inline void SetVisible( bool flag )
		{
			isVisible = flag;
		}

		[[nodiscard]] inline bool ShouldDraw() const
		{
			if ( !isVisible || ( x + w < 0 || x > 640 || y + h < 0 || y > 480 ) )
			{
				return false;
			}

			return true;
		}

	protected:
		GUIStyleSheet *styleSheet_{ nullptr };

		std::vector< GUIPanel * > children_;

		int x{ 0 }, y{ 0 };
		int w{ 640 }, h{ 480 };

		bool isDrawing{ false };// Flag on whether or not the panel is actually in view
		bool isVisible{ true }; // User flag, specifying if the panel should show or not

	private:
		void DrawBorder();
		void DrawBorderCorner( int dx, int dy, unsigned int index );
		void DrawBorderEdge( int dx, int dy, int dw, int dh, unsigned int index );

		Background background_{ Background::NONE };
		Border border_{ Border::NONE };

		static const hei::Colour INSET_COLOUR;
		static const hei::Colour OUTSET_COLOUR;

		hei::Colour backgroundColour_{ INSET_COLOUR };

		GUIPanel *parentPtr{ nullptr };

		char tooltipDescription_[ 32 ];
		GUIPanel *tooltip_{ nullptr };
		unsigned int tooltipHoverTime_{ 0 };
		static constexpr unsigned int TOOLTIP_MAX_HOVER = 100;

	public:
		void SetTooltip( const char *description );
	};
}// namespace vc
