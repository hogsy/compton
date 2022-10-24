// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace ct
{
	class Sprite;

	class GUIImage : public GUIPanel
	{
	public:
		IMPLEMENT_SUPER( GUIPanel )

		GUIImage( GUIPanel *parent = nullptr, const Sprite *sprite = nullptr, int x = 0, int y = 0, int w = 0, int h = 0 );
		~GUIImage();

		virtual void DrawBackground() override;

	private:
		const Sprite *sprite_;
	};
}// namespace ct
