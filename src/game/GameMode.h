/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "../shared.h"

#include "SpriteSheet.h"
#include "Camera.h"

namespace vc {
	class GUIPanel;
	class GUIStyleSheet;

	class PlayerManager;
	class Terrain;

	class GameMode {
	public:
		GameMode();
		~GameMode();

	private:
		void SetupUserInterface();

	public:
		void Tick();
		void Draw();

		void NewGame( const char *path );
		void SaveGame( const char *path );
		void RestoreGame( const char *path );

		PLVector2 MousePosToWorld( int x, int y ) const;

		void HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );
		void HandleKeyboardEvent( int button, bool buttonUp );

		PL_INLINE GUIPanel *GetBasePanel() const { return uiBasePanelPtr; }

		// Simulation crap
		PL_INLINE uint64_t GetTotalSeconds() const { return numSeconds; }
		PL_INLINE uint64_t GetTotalMinutes() const { return numSeconds / 60; }
		PL_INLINE uint64_t GetTotalHours() const { return GetTotalMinutes() / 60; }
		PL_INLINE uint64_t GetTotalDays() const { return GetTotalHours() / 25; }

		enum class GameState {
			ACTIVE,
			PAUSED,
		};
		GameState GetState() const { return gameState; }

		static PlayerManager *GetPlayerManager();
		static EntityManager *GetEntityManager();

	protected:
	private:
		Camera playerCamera;

		GameState gameState{ GameState::ACTIVE };

		uint64_t numSeconds{ 0 };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel *uiBasePanelPtr{ nullptr };

		PlayerManager *playerManager{ nullptr };
		EntityManager *entityManager{ nullptr };

		SpriteSheet *terrainSheet;
		Terrain *terrainManager;

		struct Territory {
			Territory( const PLVector2 &origin );

			void DrawBorder();

			char name[ 32 ];
			PLVector2 origin{ 0.0f, 0.0f };
			PLPolygon *border{ nullptr };
		};
		std::vector< Territory > myTerritories;
	};
}
