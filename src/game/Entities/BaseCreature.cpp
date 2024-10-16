// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * BaseCreature.cpp
 *  Foundation of everything with intelligence.
 *------------------------------------------------------------------------------------*/

#include "engine_private.h"
#include "../../engine/Random.h"
#include "../../engine/Serializer.h"

#include "BaseCreature.h"

vc::BaseCreature::BaseCreature()  = default;
vc::BaseCreature::~BaseCreature() = default;

void vc::BaseCreature::Spawn()
{
	SuperClass::Spawn();

	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
	{
		sensors[ i ] = ai::Sensor( ( ai::Sensor::Type )( ( uint8_t ) ai::Sensor::Type::SIGHT + i ), &brain );
	}

	// Age
	age        = random::GenerateRandomInteger( 1, 50 );
	maxAge     = random::GenerateRandomInteger( age, age + 50 );
	generation = random::GenerateRandomInteger( 1, 10 );

	// Health
	health    = random::GenerateRandomInteger( 50, 100 );
	maxHealth = random::GenerateRandomInteger( health, health + 100 );

	// Stamina
	stamina    = random::GenerateRandomInteger( 30, 100 );
	maxStamina = random::GenerateRandomInteger( stamina, stamina + 100 );

	// Generate a random sex for the creature
	sex = static_cast< Sex >( random::GenerateRandomInteger( 0, static_cast< int >( Sex::MAX_SEXES ) ) );
}

/**
 * Determines whether or not this character can breed with the other.
 */
bool vc::BaseCreature::CanBreed( BaseCreature *other )
{
	// Can't get pregnant if we're already pregnant!
	if ( isPregnant || other->isPregnant )
	{
		return false;
	}

	// Only adults can breed
	if ( phase != LifePhase::LIFE_PHASE_ADULTHOOD && other->phase != LifePhase::LIFE_PHASE_ADULTHOOD )
	{
		return false;
	}

	switch ( sex )
	{
		default: break;
		case Sex::INTERSEX:
			return true;
		case Sex::MALE:
			if ( other->sex == Sex::FEMALE )
			{
				return true;
			}
			break;
		case Sex::FEMALE:
			if ( other->sex == Sex::MALE )
			{
				return true;
			}
			break;
	}

	return false;
}

void vc::BaseCreature::Deserialize( vc::Serializer *read )
{
	SuperClass::Deserialize( read );

	health    = read->ReadInteger();
	maxHealth = read->ReadInteger();

	stamina    = read->ReadInteger();
	maxStamina = read->ReadInteger();

	experience    = read->ReadInteger();
	maxExperience = read->ReadInteger();

	sex = static_cast< Sex >( read->ReadInteger() );
}

void vc::BaseCreature::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( health );
	write->WriteInteger( maxHealth );

	write->WriteInteger( stamina );
	write->WriteInteger( maxStamina );

	write->WriteInteger( experience );
	write->WriteInteger( maxExperience );

	write->WriteInteger( static_cast< int >( sex ) );
}

void vc::BaseCreature::Draw( const vc::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	//al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 255, 0, 255 ) );
	//al_draw_pixel( origin.x, origin.y, al_map_rgb( 0, 255, 0 ) );
}

void vc::BaseCreature::Tick()
{
	SuperClass::Tick();

	// Check sensors - these will pass data to brain
	for ( auto &mySensor : sensors )
	{
		mySensor.Tick();
	}

	// Brain will now process input from sensors
	brain.Tick();

	// Now handle the actions necessary for the current directive

	const ai::Brain::Directive *directive = brain.GetTopDirective();
	if ( directive == nullptr || directive->isCompleted )
	{
		return;
	}

	switch ( directive->type )
	{
		case ai::MotorAction::USE:
			Use();
			break;
		case ai::MotorAction::DRINK:
			Drink();
			break;
		case ai::MotorAction::EAT: break;
		case ai::MotorAction::TALK: break;
		case ai::MotorAction::ATTACK: break;
		case ai::MotorAction::APPROACH:
			StepTowards( directive->targetPosition );
			break;
		case ai::MotorAction::RETREAT:
			StepAway( directive->targetPosition );
			break;
	}
}

vc::ai::FeedbackState vc::BaseCreature::Use()
{
	if ( targetEntity == nullptr )
	{
		return {};
	}

	return {};
}

vc::ai::FeedbackState vc::BaseCreature::Drink()
{
	return vc::ai::FeedbackState();
}

vc::ai::FeedbackState vc::BaseCreature::Eat()
{
	return vc::ai::FeedbackState();
}

vc::ai::FeedbackState vc::BaseCreature::Talk()
{
	return vc::ai::FeedbackState();
}

vc::ai::FeedbackState vc::BaseCreature::Attack()
{
	return vc::ai::FeedbackState();
}

void vc::BaseCreature::StepTowards( const PLVector2 &target, int speed )
{
	if ( stepTime > GetApp()->GetNumOfTicks() )
	{
		return;
	}

	if ( target.x > origin.x )
	{
		origin.x++;
	}
	else if ( target.x < origin.x )
	{
		origin.x--;
	}
	if ( target.y > origin.y )
	{
		origin.y++;
	}
	else if ( target.y < origin.y )
	{
		origin.y--;
	}

	stepTime = GetApp()->GetNumOfTicks() + speed;
}

void vc::BaseCreature::StepAway( const PLVector2 &target, int speed )
{
	if ( stepTime > GetApp()->GetNumOfTicks() )
	{
		return;
	}

	if ( target.x >= origin.x )
	{
		origin.x--;
	}
	else if ( target.x <= origin.x )
	{
		origin.x++;
	}
	if ( target.y >= origin.y )
	{
		origin.y--;
	}
	else if ( target.y <= origin.y )
	{
		origin.y++;
	}

	stepTime = GetApp()->GetNumOfTicks() + speed;
}

bool vc::BaseCreature::IsMoving() const
{
	const ai::Brain::Directive *directive = brain.GetTopDirective();
	if ( directive == nullptr )
	{
		return false;
	}

	return ( directive->type == ai::MotorAction::APPROACH || directive->type == ai::MotorAction::RETREAT );
}
