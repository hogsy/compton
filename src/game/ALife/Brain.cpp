// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "engine/Compton.h"

#include "Brain.h"

ct::ai::Brain::Brain()  = default;
ct::ai::Brain::~Brain() = default;

void ct::ai::Brain::Tick()
{
	if ( !active )
		return;

	assert( owner_ != nullptr );

	if ( directives_.empty() )
	{
		Directive idle;
		idle.type             = MotorAction::APPROACH;
		idle.weight           = 100.0;
		idle.targetPosition.x = owner_->origin_.x + ( ( rand() % 32 ) - rand() % 64 );
		idle.targetPosition.y = owner_->origin_.y + ( ( rand() % 32 ) - rand() % 64 );
		idle.description      = "idle";
		directives_.push_back( idle );
	}
	else if ( rand() % 100 == 50 )
		directives_.clear();

	const ai::Brain::Directive *directive = GetTopDirective();
	if ( directive == nullptr || directive->isCompleted )
		return;

	switch ( directive->type )
	{
		default:
			break;
		case MotorAction::APPROACH:
			//todo: check if we're in proximity
			break;
	}
}
