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

#include "../shared.h"

#include "PhysicsAgent.h"
#include "game.h"

#define DEFAULT_GRAVITY 0.085f

void PhysicsAgent::SetupProperties( const AgentDefinitionLoader &adf_loader )
{
	SpriteAgent::SetupProperties( adf_loader );

	use_physics_ = adf_loader.GetProperty( "physics" ) == "true";
	elasticity	 = std::strtof( adf_loader.GetProperty( "elasticity", "0" ).c_str(), nullptr );
}

void PhysicsAgent::TickPhysics()
{
	if ( !use_physics_ )
	{
		return;
	}

	// gravity
	velocity_.y += DEFAULT_GRAVITY;

	if ( velocity_.x > 0 )
	{
		velocity_.x -= 0.05f;
	}
	else if ( velocity_.x < 0 )
	{
		velocity_.x += 0.05f;
	}

	if ( is_grounded_ )
	{
		if ( velocity_.x < 0.05f && velocity_.x > -0.05f )
		{
			velocity_.x = 0;
		}
	}

	if ( ( position_.x <= 0 ) || ( position_.x >= 63 ) )
	{
		velocity_.x *= -1;
	}

	position_ += velocity_;

	if ( position_.y < 0 )
	{
		position_.y = 0;
		velocity_.y *= -1;
	}

	if ( position_.y >= ( 128 + bounds_.x - 2 ) )
	{
		velocity_.y *= ( -1 * elasticity );
		position_.y = ( 128 + bounds_.y - 2 );
		Impact( nullptr );
		is_grounded_ = true;
	}
	else
	{
		is_grounded_ = false;
	}

	if ( position_.x < 0 )
	{
		position_.x = 0;
		//  velocity_.x *= -1;
	}
	else if ( position_.x > DISPLAY_WIDTH - 1 )
	{
		position_.x = DISPLAY_WIDTH - 1;
		//  velocity_.x *= -1;
	}
}

void PhysicsAgent::Impact( Agent *agent )
{
	if ( velocity_.Length() <= 0.2f )
	{
		return;
	}

	float gain	= plClamp( 0, velocity_.Length() * 1.0f, 1 );
	float speed = plClamp( 0.5f, velocity_.Length() * 1.0f, 2 );

	//printf( "speed: %f\n", speed );
	//printf( "gain: %f\n", gain );

	//al_play_sample( game.sample_land, gain, ALLEGRO_AUDIO_PAN_NONE, speed, ALLEGRO_PLAYMODE_ONCE, nullptr );
}

void PhysicsAgent::Tick()
{
	TickPhysics();

	SpriteAgent::Tick();
}

void PhysicsAgent::Draw()
{
	SpriteAgent::Draw();
}
