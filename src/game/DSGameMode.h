// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "engine/GameMode.h"

#include "engine/GUI/GUIPanel.h"
#include "engine/GUI/GUIPieMenu.h"

#include "engine/SpriteSheet.h"
#include "engine/Camera.h"
#include "engine/PlayerManager.h"
#include "engine/Input/Action.h"

#include "World.h"

namespace ct
{
	class PlayerManager;
	class Terrain;
	class Background;

	class DSGameMode : public IGameMode
	{
	public:
		DSGameMode();
		~DSGameMode();

		void Precache() override;

	private:
		virtual void SetupDesktop();

	public:
		void Tick() override;
		void Draw() override;

		void NewGame( const char *path ) override;
		void SaveGame( const char *path ) override;
		void RestoreGame( const std::string &path ) override;

		hei::Vector2 MousePosToWorld( int x, int y );

		bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp ) override;
		bool HandleKeyboardEvent( int button, bool buttonUp ) override;

		inline GUIPanel *GetBasePanel() const { return baseGuiPanel_; }

		////////////////////////////////////////////////

		enum class GameState
		{
			ACTIVE,
			PAUSED,
		};
		GameState GetState() const { return gameState_; }

	private:
		GameState gameState_{ GameState::ACTIVE };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel      *baseGuiPanel_{ nullptr };
		GUIPieMenu    *uiPieMenu{ nullptr };
		GUILabel      *dayLabel{ nullptr };

		World *world_{ nullptr };

	public:
		[[nodiscard]] World *GetWorld() const
		{
			return world_;
		}

		////////////////////////////////////////////////
		// Actions

		enum Action
		{
			ACTION_MOVE_UP,
			ACTION_MOVE_DOWN,
			ACTION_MOVE_LEFT,
			ACTION_MOVE_RIGHT,

			ACTION_USE,
			ACTION_ATTACK,

			MAX_ACTIONS
		};

		void RegisterActions() override;

	private:
		input::Action *actions_[ Action::MAX_ACTIONS ];

	public:
		[[nodiscard]] inline const input::Action *GetAction( Action slot ) const
		{
			return actions_[ slot ];
		}
	};
}// namespace ct
