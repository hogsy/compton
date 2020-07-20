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

	class GameMode {
	public:
		GameMode();
		~GameMode();

		void Tick();

		void SaveGame();
		void RestoreGame();

		void HandleMouseEvent( int x, int y, int button, bool buttonUp );
		void HandleKeyboardEvent( int button, bool buttonUp );

	protected:
	private:
		Camera playerCamera;

		enum class GameState {
			ACTIVE,
			PAUSED,
		};
		GameState curGameState{ GameState::ACTIVE };

		SpriteSheet *terrainSheet;
		SpriteSheet *interfaceSheet;

		GUIPanel *uiPanelPtr{ nullptr };
	};
}
