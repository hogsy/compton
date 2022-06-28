// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/GameMode.h"

namespace ct
{
	class SpriteSheetEditor : public IGameMode
	{
	public:
		SpriteSheetEditor() {}
		~SpriteSheetEditor() {}

		void NewGame( const char *path ) override;
		void SaveGame( const char *path ) override;
		void RestoreGame( const char *path ) override;

		void Tick() override;
		void Draw() override;

		void Precache() override;

		bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp ) override;
		bool HandleKeyboardEvent( int key, bool keyUp ) override;

	private:
		std::string document_;
	};
}
