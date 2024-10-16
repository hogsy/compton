// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "camera.h"

class PlayerManager;
class Background;

class GUIPanel;
class GUIStyleSheet;

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

	[[nodiscard]] hei::Vector2 MousePosToWorld( int x, int y ) const;

	bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp );
	bool HandleKeyboardEvent( int button, bool buttonUp );

	[[nodiscard]] inline GUIPanel *GetBasePanel() const { return baseGuiPanel_; }

	// Simulation crap (todo: move into world class)

	[[nodiscard]] inline uint64_t GetTotalSeconds() const { return numSeconds; }
	[[nodiscard]] inline uint64_t GetTotalMinutes() const { return numSeconds / 60; }
	[[nodiscard]] inline uint64_t GetTotalHours() const { return GetTotalMinutes() / 60; }
	[[nodiscard]] inline uint64_t GetTotalDays() const { return GetTotalHours() / 24; }

	[[nodiscard]] inline unsigned int GetCurrentSecond() const
	{
		return ( GetTotalSeconds() - ( GetTotalMinutes() / 60 ) ) % 60;
	}
	[[nodiscard]] inline unsigned int GetCurrentMinute() const
	{
		return ( GetTotalMinutes() - ( GetTotalHours() / 60 ) ) % 60;
	}
	[[nodiscard]] inline unsigned int GetCurrentHour() const
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
	[[nodiscard]] inline TimeOfDay GetTimeOfDay() const
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
	[[nodiscard]] GameState GetState() const { return gameState; }

	static PlayerManager *GetPlayerManager();
	static EntityManager *GetEntityManager();
	static Background    *GetBackgroundManager();

private:
	Camera playerCamera;

	GameState gameState{ GameState::ACTIVE };

	uint64_t numSeconds{};

	GUIStyleSheet *uiDefaultStyleSheet{};
	GUIPanel      *baseGuiPanel_{};

	PlayerManager *playerManager{};
	EntityManager *entityManager_{};

	Background *backgroundManager_{};

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
	void DrawRoomsDebug( const Camera &camera );

public:
	[[nodiscard]] const Room *GetRoom( int x, int y ) const;
	[[nodiscard]] const Room *GetRoomByType( uint16_t type, unsigned int startIndex = 0 ) const;
	[[nodiscard]] const Room *GetRoomByIndex( unsigned int index ) const;

private:
	bool enableRoomDraw_{ false };
	bool enableHelpPrompt_{ true };
};

GameMode *GetGameModeInterface();
