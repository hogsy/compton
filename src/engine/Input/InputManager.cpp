// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "InputManager.h"
#include "GameMode.h"

using namespace ct;

input::Action *input::InputManager::PushAction( const char *description )
{
	// Could've probably used a std::map for this...
	for ( auto &i : actions_ )
	{
		if ( strcmp( i.GetDescription(), description ) != 0 )
		{
			continue;
		}

		return &i;
	}

	actions_.push_back( Action( description ) );
	return &actions_[ actions_.size() - 1 ];
}

bool input::InputManager::HandleKeyboardEvent( int key, bool keyUp )
{
	keys_[ key ] = keyUp ? State::RELEASED : State::PRESSED;

	GameMode *gameMode = App::GetGameMode();
	if ( gameMode != nullptr && gameMode->GetBasePanel() != nullptr )
	{
		GUIPanel *basePanel = gameMode->GetBasePanel();
		locked_ = true;
		return basePanel->HandleKeyboardEvent( key, keyUp );
	}

	return false;
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

	mouseButtons_[ button ] = buttonUp ? State::RELEASED : State::PRESSED;

	GameMode *gameMode = App::GetGameMode();
	if ( gameMode != nullptr && gameMode->GetBasePanel() != nullptr )
	{
		GUIPanel *basePanel = gameMode->GetBasePanel();
		locked_ = true;
		return basePanel->HandleMouseEvent( x, y, wheel, button, buttonUp );
	}

	return false;
}
