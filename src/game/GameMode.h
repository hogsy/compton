// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "GUI/GUIPanel.h"
#include "GUI/GUIPieMenu.h"

#include "SpriteSheet.h"
#include "Camera.h"
#include "World.h"

#include "Input/Action.h"

namespace ct
{
	class PlayerManager;
	class Terrain;
	class Background;

	class GameMode
	{
	public:
		GameMode();
		~GameMode();

		virtual void PrecacheResources();

	private:
		void SetupUserInterface();

	public:
		void Tick();
		void Draw();

		virtual void NewGame( const char *path );
		virtual void SaveGame( const char *path );
		virtual void RestoreGame( const char *path );

		hei::Vector2 MousePosToWorld( int x, int y ) const;

		virtual void HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );
		virtual void HandleKeyboardEvent( int button, bool buttonUp );

		inline GUIPanel *GetBasePanel() const { return baseGuiPanel_; }

		////////////////////////////////////////////////

		enum class GameState
		{
			ACTIVE,
			PAUSED,
		};
		GameState GetState() const { return gameState_; }

		static PlayerManager *GetPlayerManager();
		static EntityManager *GetEntityManager();
		static Background    *GetBackgroundManager();

		unsigned int GetGameSpeed() const { return gameSpeed_; }

	private:
		Camera camera;

		GameState gameState_{ GameState::ACTIVE };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel      *baseGuiPanel_{ nullptr };
		GUIPieMenu    *uiPieMenu{ nullptr };

		PlayerManager *playerManager;
		EntityManager *entityManager_;

		SpriteSheet *terrainSheet;
		Background  *backgroundManager_;
		World       *world_{ nullptr };

		bool enableHelpPrompt_{ true };

		unsigned int gameSpeed_{ 5 };

		////////////////////////////////////////////////
		// Actions

		enum
		{
			ACTION_MOVE_UP,
			ACTION_MOVE_DOWN,
			ACTION_MOVE_LEFT,
			ACTION_MOVE_RIGHT,

			ACTION_USE,
			ACTION_ATTACK,

			MAX_ACTIONS
		};

	public:
		void RegisterActions();

	private:
		std::array< input::Action *, MAX_ACTIONS > actions_;
	};
}// namespace vc
