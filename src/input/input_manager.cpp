// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../app.h"
#include "../game_mode.h"

#include "input.h"

input::Action *input::InputManager::CreateAction( const char *description )
{
	auto i = actions_.find( description );
	if ( i != actions_.end() )
	{
		return &i->second;
	}

	auto j = actions_.insert( std::make_pair< std::string, Action >( description, Action( description ) ) );
	return &j.first->second;
}

bool input::InputManager::HandleKeyboardEvent( int key, bool keyUp )
{
	keys[ key ] = keyUp ? State::RELEASED : State::PRESSED;

	GameMode *gameMode = GetGameModeInterface();
	if ( gameMode == nullptr )
	{
		return false;
	}

	locked = true;
	return gameMode->HandleKeyboardEvent( key, keyUp );
}

bool input::InputManager::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	ox_ = mx_;
	oy_ = my_;
	oz_ = mz_;

	mx_ = x;
	my_ = y;
	mz_ = wheel;

	dx_ = mx_ - ox_;
	dy_ = my_ - oy_;
	dz_ = mz_ - oz_;

	mouseButtons[ button ] = buttonUp ? State::RELEASED : State::PRESSED;

	GameMode *gameMode = GetGameModeInterface();
	if ( gameMode == nullptr )
	{
		return false;
	}

	locked = true;
	return gameMode->HandleMouseEvent( x, y, wheel, button, buttonUp );
}
