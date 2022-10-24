// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Camera.h"

namespace ct
{
	class PlayerManager
	{
	public:
		struct Player
		{
			Player( const std::string &sName, bool local ) : name( sName ), isLocal( local ) {}
			Camera      camera;
			bool        isLocal{ true };
			std::string name;
			Entity     *controlTarget{ nullptr };
		};

		int  AddPlayer( const char *name, bool local );
		void RemovePlayer( int id );

		void HandleInput();

		inline const std::vector< Player > *GetPlayers() const { return &players_; }
		[[nodiscard]] inline int            GetNumPlayers() const { return ( int ) players_.size(); }

		Player *GetPlayer( int id );

	private:
		std::vector< Player > players_;
	};
}// namespace ct
