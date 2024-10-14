// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "../engine/GUI/GUIPanel.h"
#include "../engine/GUI/GUIPieMenu.h"

#include "../engine/SpriteSheet.h"
#include "../engine/Camera.h"

namespace vc
{
	class PlayerManager;
	class Terrain;
	class Background;

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

		inline GUIPanel *GetBasePanel() const { return baseGuiPanel_; }

		// Simulation crap (todo: move into world class)

		inline uint64_t GetTotalSeconds() const { return numSeconds; }
		inline uint64_t GetTotalMinutes() const { return numSeconds / 60; }
		inline uint64_t GetTotalHours() const { return GetTotalMinutes() / 60; }
		inline uint64_t GetTotalDays() const { return GetTotalHours() / 24; }

		inline unsigned int GetCurrentSecond() const
		{
			return ( GetTotalSeconds() - ( GetTotalMinutes() / 60 ) ) % 60;
		}
		inline unsigned int GetCurrentMinute() const
		{
			return ( GetTotalMinutes() - ( GetTotalHours() / 60 ) ) % 60;
		}
		inline unsigned int GetCurrentHour() const
		{
			return ( GetTotalHours() - ( GetTotalDays() / 24 ) ) % 24;
		}

		enum class TimeOfDay
		{
			DAWN,
			MORNING,
			AFTERNOON,
			EVENING,
			NIGHT
		};
		inline TimeOfDay GetTimeOfDay() const
		{
			unsigned int curHour = GetCurrentHour();
			if ( curHour > 17 )
			{
				return TimeOfDay::NIGHT;
			}
			else if ( curHour > 15 )
			{
				return TimeOfDay::EVENING;
			}
			else if ( curHour > 12 )
			{
				return TimeOfDay::AFTERNOON;
			}
			else if ( curHour > 9 )
			{
				return TimeOfDay::MORNING;
			}
			else if ( curHour > 5 )
			{
				return TimeOfDay::DAWN;
			}

			return TimeOfDay::NIGHT;
		}

		////////////////////////////////////////////////

		enum class GameState
		{
			ACTIVE,
			PAUSED,
		};
		GameState GetState() const { return gameState; }

		static PlayerManager *GetPlayerManager();
		static EntityManager *GetEntityManager();
		static Background    *GetBackgroundManager();

	private:
		Camera playerCamera;

		GameState gameState{ GameState::ACTIVE };

		uint64_t numSeconds{ 0 };

		GUIStyleSheet *uiDefaultStyleSheet;
		GUIPanel      *baseGuiPanel_{ nullptr };
		GUIPieMenu    *uiPieMenu{ nullptr };

		PlayerManager *playerManager{ nullptr };
		EntityManager *entityManager_{ nullptr };

		Background *backgroundManager_;

		// Rooms

	public:
		struct Room
		{
			enum Type
			{
				ROOM_TYPE_INTERIOR,
				ROOM_TYPE_EXTERIOR,
			};

			int      x;
			int      w;
			int      y;
			int      h;
			uint16_t type;

			unsigned int id;
		};

	private:
		std::vector< Room > rooms_;

		void LoadRooms();
		void DrawRoomsDebug( const vc::Camera &camera );

	public:
		const Room *GetRoom( int x, int y ) const;
		const Room *GetRoomByType( uint16_t type, unsigned int startIndex = 0 ) const;
		const Room *GetRoomByIndex( unsigned int index ) const;

	private:
		bool enableRoomDraw_{ false };
		bool enableHelpPrompt_{ true };

		///////////////////////////////////////////////

		struct Territory
		{
			Territory( const hei::Vector2 &origin );

			void DrawBorder();

			char         name[ 32 ];
			hei::Vector2 origin{ 0.0f, 0.0f };
			PLGPolygon  *border{ nullptr };
		};
		std::vector< Territory > myTerritories;
	};
}// namespace vc
