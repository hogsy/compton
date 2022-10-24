// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../GameMode.h"

#include "../GUI/GUIPanel.h"

namespace ct
{
	class SpriteSheetEditor : public IGameMode
	{
	public:
		SpriteSheetEditor();
		~SpriteSheetEditor();

		void NewGame( const char *path ) override;
		void SaveGame( const char *path ) override;
		void RestoreGame( const std::string &path ) override;

		void Tick() override;
		void Draw() override;

		void Precache() override;

		bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp ) override;
		bool HandleKeyboardEvent( int key, bool keyUp ) override;

	private:
		std::string document_;

		enum class State
		{
			STATE_NONE,
			STATE_OPEN,
			STATE_SAVE,
			STATE_DOCUMENT,
		};

		State state_{ State::STATE_NONE };

		bool lmState_{ false };
		bool rmState_{ false };

		GUIPanel      *basePanel_;
		GUIStyleSheet *styleSheet_;

		static constexpr unsigned int LIST_W = DISPLAY_WIDTH;
		static constexpr unsigned int LIST_H = 128;
		static constexpr unsigned int LIST_X = DISPLAY_HEIGHT - LIST_H;

		static constexpr unsigned int VIEWPORT_X = 0;
		static constexpr unsigned int VIEWPORT_Y = 0;
		static constexpr unsigned int VIEWPORT_W = DISPLAY_WIDTH;
		static constexpr unsigned int VIEWPORT_H = DISPLAY_HEIGHT - LIST_H;
	};
}// namespace ct
