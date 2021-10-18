/*
SimGame Engine
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

#pragma once

#include "SpriteAgent.h"

class PhysicsAgent : public SpriteAgent
{
public:
	IMPLEMENT_FACTORY( PhysicsAgent )

	void SetupProperties( const AgentDefinitionLoader &adf_loader ) override;

	void Tick() override;
	void Draw() override;

	virtual void Impact( Agent *agent );

	PLVector2	 GetVelocity() { return velocity_; }
	virtual void SetVelocity( PLVector2 vel )
	{
		old_velocity_ = velocity_;
		velocity_	  = vel;
	}

protected:
	PLVector2 velocity_, old_velocity_;

private:
	void TickPhysics();

	PLVector2 bounds_{ 0, 0 };

	float elasticity{ 0 };

	bool use_physics_{ false };
	bool is_grounded_{ false };
};
