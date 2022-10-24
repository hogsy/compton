// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "PlayerManager.h"
#include "EntityManager.h"

namespace ct
{
	class IGameMode
	{
	public:
		virtual inline ~IGameMode() = default;

		virtual void NewGame( const char *path )            = 0;
		virtual void SaveGame( const char *path )           = 0;
		virtual void RestoreGame( const std::string &path ) = 0;

		virtual void Tick() = 0;
		virtual void Draw() = 0;

		virtual void Precache() {}

		virtual bool HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp ) = 0;
		virtual bool HandleKeyboardEvent( int key, bool keyUp )                             = 0;

		virtual void RegisterActions() {}

		inline PlayerManager *GetPlayerManager() { return &playerManager_; }
		inline EntityManager *GetEntityManager() { return &entityManager_; }

		inline bool IsPaused() const { return isPaused_; }

		inline unsigned int GetSpeed() const { return speed_; }

	protected:
		PlayerManager playerManager_;
		EntityManager entityManager_;

		bool isPaused_{ false };

		unsigned int speed_{ 5 };
	};
}// namespace ct
