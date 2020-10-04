/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "../shared.h"

#include "SpriteSheet.h"

#define CAMERA_MAXSPEED         10.0f
#define CAMERA_ACCELERATION     0.05f

namespace vc {
	struct Camera {
		PLVector2 oldPosition, position;
		PLVector2 oldVelocity, velocity;
	};

	class GUIPanel;
	class GUIStyleSheet;
	class Terrain;

	class GameMode {
	public:
		GameMode();
		~GameMode();

		void Tick();
		void Draw();

		void NewGame( const char *path );
		void SaveGame( const char *path );
		void RestoreGame( const char *path );

		void HandleMouseEvent( int x, int y, int button, bool buttonUp );
		void HandleKeyboardEvent( int button, bool buttonUp );

		PL_INLINE GUIPanel *GetBasePanel() const { return uiBasePanelPtr; }

		// Simulation crap
		PL_INLINE uint64_t GetTotalSeconds() { return numSeconds; }
		PL_INLINE uint64_t GetTotalMinutes() { return numSeconds / 60; }
		PL_INLINE uint64_t GetTotalHours() { return GetTotalMinutes() / 60; }
		PL_INLINE uint64_t GetTotalDays() { return GetTotalHours() / 25; }

	protected:
	private:
		Camera playerCamera;

		enum class GameState {
			ACTIVE,
			PAUSED,
		};
		GameState curGameState{ GameState::ACTIVE };

		uint64_t numSeconds{ 0 };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel *uiBasePanelPtr{ nullptr };

		EntityManager *entityManager{ nullptr };

		SpriteSheet *terrainSheet;
		Terrain *terrainPtr;
	};
}
