// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../../app.h"

#include "Brain.h"

Brain::Brain()  = default;
Brain::~Brain() = default;

void Brain::Tick()
{
	if ( !active )
	{
		return;
	}

	assert( owner_ != nullptr );

	if ( directives_.empty() )
	{
		Directive idle;
		idle.type             = MotorAction::APPROACH;
		idle.weight           = 100.0;
		idle.targetPosition.x = owner_->origin.x + ( ( rand() % 32 ) - rand() % 64 );
		idle.targetPosition.y = owner_->origin.y + ( ( rand() % 32 ) - rand() % 64 );
		idle.description      = "idle";
		directives_.push_back( idle );
	}
	else if ( rand() % 100 == 50 )
	{
		directives_.clear();
	}

	const Brain::Directive *directive = GetTopDirective();
	if ( directive == nullptr || directive->isCompleted )
	{
		return;
	}

	switch ( directive->type )
	{
		default:
			break;
		case MotorAction::APPROACH:
			//todo: check if we're in proximity
			break;
	}
}
