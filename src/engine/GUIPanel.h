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
			SOLID
		};
		enum class Border {
			NONE,
			INSET,
			OUTSET
		};

		GUIPanel( GUIPanel *parent = nullptr, int x = 0, int y = 0, int w = 640, int h = 480, Background background = Background::NONE, Border border = Border::NONE );
		~GUIPanel();

		void Draw();
		void DrawBackground();

		void Tick();

		void SetStyleSheet( const GUIStyleSheet *styleSheet );
		const GUIStyleSheet *GetStyle() const { return myStyleSheet; }

	protected:
	private:
		Background myBackground{ Background::NONE };
		Border myBorder{ Border::NONE };

		GUIPanel *parentPtr{ nullptr };
		const GUIStyleSheet *myStyleSheet{ nullptr };

		int x{ 0 }, y{ 0 };
		int w{ 640 }, h{ 480 };

		std::vector< GUIPanel * > children;
	};
}
