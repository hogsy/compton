/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/*--------------------------------------------------------------------------------------
 * BaseCreature.cpp
 *  Foundation of everything with intelligence.
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "BaseCreature.h"
#include "Random.h"
#include "Serializer.h"

vc::BaseCreature::BaseCreature()
{
	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
	{
		mySensors[ i ] = ai::Sensor(
				( ai::Sensor::Type )( ( uint8_t ) ai::Sensor::Type::SIGHT + i ),
				&myBrain );
	}
}

vc::BaseCreature::~BaseCreature() {}

void vc::BaseCreature::Spawn()
{
	SuperClass::Spawn();

	// Age
	age		   = random::GenerateRandomInteger( 1, 50 );
	maxAge	   = random::GenerateRandomInteger( age, age + 50 );
	generation = random::GenerateRandomInteger( 1, 10 );

	// Health
	myHealth	= random::GenerateRandomInteger( 50, 100 );
	myMaxHealth = random::GenerateRandomInteger( myHealth, myHealth + 100 );

	// Stamina
	stamina	   = random::GenerateRandomInteger( 30, 100 );
	maxStamina = random::GenerateRandomInteger( stamina, stamina + 100 );

	// Generate a random sex for the creature
	mySex = static_cast< Sex >( random::GenerateRandomInteger( 0, static_cast< int >( Sex::MAX_SEXES ) ) );
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

	switch ( mySex )
	{
		default: break;
		case Sex::INTERSEX:
			return true;
		case Sex::MALE:
			if ( other->mySex == Sex::FEMALE )
			{
				return true;
			}
			break;
		case Sex::FEMALE:
			if ( other->mySex == Sex::MALE )
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

	myHealth	= read->ReadInteger();
	myMaxHealth = read->ReadInteger();

	stamina	   = read->ReadInteger();
	maxStamina = read->ReadInteger();

	experience	  = read->ReadInteger();
	maxExperience = read->ReadInteger();

	mySex = static_cast< Sex >( read->ReadInteger() );
}

void vc::BaseCreature::Serialize( vc::Serializer *write )
{
	SuperClass::Serialize( write );

	write->WriteInteger( myHealth );
	write->WriteInteger( myMaxHealth );

	write->WriteInteger( stamina );
	write->WriteInteger( maxStamina );

	write->WriteInteger( experience );
	write->WriteInteger( maxExperience );

	write->WriteInteger( static_cast< int >( mySex ) );
}

void vc::BaseCreature::Draw( const vc::Camera &camera )
{
	SuperClass::Draw( camera );

	if ( !ShouldDraw( camera ) )
	{
		return;
	}

	al_draw_filled_circle( origin.x, origin.y, 10.0f, al_map_rgb( 255, 0, 255 ) );
	al_draw_pixel( origin.x, origin.y, al_map_rgb( 0, 255, 0 ) );
}

void vc::BaseCreature::Tick()
{
	SuperClass::Tick();

	// Check sensors - these will pass data to brain
	for ( unsigned int i = 0; i < ai::Sensor::MAX_SENSOR_TYPES; ++i )
	{
		mySensors[ i ].Tick();
	}

	// Brain will now process input from sensors
	myBrain.Tick();

	ai::Brain::Mood mood = myBrain.GetCurrentMood();
}
