// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * BaseCreature.cpp
 *  Foundation of everything with intelligence.
 *------------------------------------------------------------------------------------*/

#include "engine/Compton.h"
#include "engine/Random.h"
#include "engine/Serializer.h"

#include "BaseCreature.h"

ct::BaseCreature::BaseCreature()
{
	brain_.owner_ = this;
}

ct::BaseCreature::~BaseCreature() {}

void ct::BaseCreature::Spawn()
{
	SuperClass::Spawn();

	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
		sensors_[ i ] = ai::Sensor( ( ai::Sensor::Type )( ( uint8_t ) ai::Sensor::Type::SIGHT + i ), &brain_ );

	// Age
	age_ = random::GenerateRandomInteger( 1, 50 );
	maxAge_ = random::GenerateRandomInteger( age_, age_ + 50 );
	generation_ = random::GenerateRandomInteger( 1, 10 );

	// Health
	health_ = random::GenerateRandomInteger( 50, 100 );
	maxHealth_ = random::GenerateRandomInteger( health_, health_ + 100 );

	// Stamina
	stamina_ = random::GenerateRandomInteger( 30, 100 );
	maxStamina_ = random::GenerateRandomInteger( stamina_, stamina_ + 100 );

	// Generate a random sex for the creature
	sex_ = static_cast< Sex >( random::GenerateRandomInteger( 0, static_cast< int >( Sex::MAX_SEXES ) ) );
}

/**
 * Determines whether or not this character can breed with the other.
 */
bool ct::BaseCreature::CanBreed( BaseCreature *other )
{
	// Can't get pregnant if we're already pregnant!
	if ( isPregnant_ || other->isPregnant_ )
		return false;

	// Only adults can breed
	if ( phase_ != LifePhase::LIFE_PHASE_ADULTHOOD && other->phase_ != LifePhase::LIFE_PHASE_ADULTHOOD )
		return false;

	switch ( sex_ )
	{
		default: break;
		case Sex::INTERSEX:
			return true;
		case Sex::MALE:
			if ( other->sex_ == Sex::FEMALE )
				return true;

			break;
		case Sex::FEMALE:
			if ( other->sex_ == Sex::MALE )
				return true;

			break;
	}

	return false;
}

void ct::BaseCreature::Deserialize( ct::Serializer *read )
{
	SuperClass::Deserialize( read );

	health_ = read->ReadI32();
	maxHealth_ = read->ReadI32();

	stamina_ = read->ReadI32();
	maxStamina_ = read->ReadI32();

	experience = read->ReadI32();
	maxExperience = read->ReadI32();

	sex_ = static_cast< Sex >( read->ReadI32() );
}

void ct::BaseCreature::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteI32( health_ );
	write->WriteI32( maxHealth_ );

	write->WriteI32( stamina_ );
	write->WriteI32( maxStamina_ );

	write->WriteI32( experience );
	write->WriteI32( maxExperience );

	write->WriteI32( static_cast< int >( sex_ ) );
}

void ct::BaseCreature::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );
}

void ct::BaseCreature::Tick()
{
	SuperClass::Tick();

	Think();
}

void ct::BaseCreature::Think()
{
	// Check sensors - these will pass data to brain
	for (auto & sensor : sensors_)
		sensor.Tick();

	// Brain will now process input from sensors
	brain_.Tick();

	// Now handle the actions necessary for the current directive

	const ai::Brain::Directive *dir = brain_.GetTopDirective();
	if ( dir == nullptr || dir->isCompleted )
		return;

	switch( dir->type )
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
			StepTowards( dir->targetPosition );
			break;
		case ai::MotorAction::RETREAT:
			StepAway( dir->targetPosition );
			break;
	}
}

ct::ai::FeedbackState ct::BaseCreature::Use()
{
	if ( targetEntity_ == nullptr )
		return {};

	return {};
}

ct::ai::FeedbackState ct::BaseCreature::Drink()
{
	return ct::ai::FeedbackState();
}

ct::ai::FeedbackState ct::BaseCreature::Eat()
{
	return ct::ai::FeedbackState();
}

ct::ai::FeedbackState ct::BaseCreature::Talk()
{
	return ct::ai::FeedbackState();
}

ct::ai::FeedbackState ct::BaseCreature::Attack()
{
	return ct::ai::FeedbackState();
}

void ct::BaseCreature::StepTowards( const hei::Vector2 &target, float speed )
{
	hei::Vector2 d = hei::Vector2( target - origin_ );
	origin_ += d * ( speed / ( d.Length() + 1.0f ) );
}

void ct::BaseCreature::StepAway( const hei::Vector2 &target, int speed )
{
	hei::Vector2 d = hei::Vector2( target - origin_ );
	origin_ -= d * ( speed / ( d.Length() + 1.0f ) );
}
