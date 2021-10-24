/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "../shared.h"

#include "GUI/GUIPanel.h"
#include "GUI/GUIPieMenu.h"

#include "SpriteSheet.h"
#include "Camera.h"

namespace vc
{
	class PlayerManager;
	class Terrain;

	class GameMode
	{
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

		hei::Vector2 MousePosToWorld( int x, int y ) const;

		void HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );
		void HandleKeyboardEvent( int button, bool buttonUp );

		inline GUIPanel *GetBasePanel() const { return uiBasePanelPtr; }

		// Simulation crap
		inline uint64_t GetTotalSeconds() const { return numSeconds; }
		inline uint64_t GetTotalMinutes() const { return numSeconds / 60; }
		inline uint64_t GetTotalHours() const { return GetTotalMinutes() / 60; }
		inline uint64_t GetTotalDays() const { return GetTotalHours() / 25; }

		enum class GameState
		{
			ACTIVE,
			PAUSED,
		};
		GameState GetState() const { return gameState; }

		static PlayerManager *GetPlayerManager();
		static EntityManager *GetEntityManager();
		static Terrain *	  GetTerrainManager();

	protected:
	private:
		Camera playerCamera;

		GameState gameState{ GameState::ACTIVE };

		uint64_t numSeconds{ 0 };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel *	   uiBasePanelPtr{ nullptr };
		GUIPieMenu *   uiPieMenu{ nullptr };

		PlayerManager *playerManager{ nullptr };
		EntityManager *entityManager{ nullptr };

		SpriteSheet *terrainSheet;
		Terrain *	 terrainManager;

		struct Territory
		{
			Territory( const hei::Vector2 &origin );

			void DrawBorder();

			char		name[ 32 ];
			hei::Vector2	origin{ 0.0f, 0.0f };
			PLGPolygon *border{ nullptr };
		};
		std::vector< Territory > myTerritories;
	};
}// namespace vc
