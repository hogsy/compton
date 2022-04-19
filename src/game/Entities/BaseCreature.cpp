// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>
/*--------------------------------------------------------------------------------------
 * BaseCreature.cpp
 *  Foundation of everything with intelligence.
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "BaseCreature.h"
#include "Random.h"
#include "Serializer.h"

ct::BaseCreature::BaseCreature()
{
	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
	{
		sensors_[ i ] = ai::Sensor(
				( ai::Sensor::Type )( ( uint8_t ) ai::Sensor::Type::SIGHT + i ),
				&brain_ );
	}
}

ct::BaseCreature::~BaseCreature() {}

void ct::BaseCreature::Spawn()
{
	SuperClass::Spawn();

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
	{
		return false;
	}

	switch ( sex_ )
	{
		default: break;
		case Sex::INTERSEX:
			return true;
		case Sex::MALE:
			if ( other->sex_ == Sex::FEMALE )
			{
				return true;
			}
			break;
		case Sex::FEMALE:
			if ( other->sex_ == Sex::MALE )
			{
				return true;
			}
			break;
	}

	return false;
}

void ct::BaseCreature::Deserialize( ct::Serializer *read )
{
	SuperClass::Deserialize( read );

	health_ = read->ReadInteger();
	maxHealth_ = read->ReadInteger();

	stamina_ = read->ReadInteger();
	maxStamina_ = read->ReadInteger();

	experience    = read->ReadInteger();
	maxExperience = read->ReadInteger();

	sex_ = static_cast< Sex >( read->ReadInteger() );
}

void ct::BaseCreature::Serialize( ct::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( health_ );
	write->WriteInteger( maxHealth_ );

	write->WriteInteger( stamina_ );
	write->WriteInteger( maxStamina_ );

	write->WriteInteger( experience );
	write->WriteInteger( maxExperience );

	write->WriteInteger( static_cast< int >( sex_ ) );
}

void ct::BaseCreature::Draw( const ct::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	//al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 255, 0, 255 ) );
	//al_draw_pixel( origin.x, origin.y, al_map_rgb( 0, 255, 0 ) );
}

void ct::BaseCreature::Tick()
{
	SuperClass::Tick();

	// Check sensors - these will pass data to brain
	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
	{
		sensors_[ i ].Tick();
	}

	// Brain will now process input from sensors
	brain_.Tick();

	ai::Brain::Mood mood = brain_.GetCurrentMood();
}
