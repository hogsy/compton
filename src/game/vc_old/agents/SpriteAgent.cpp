// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"
#include "SpriteAgent.h"

void SpriteAgent::SetupProperties( const AgentDefinitionLoader &adf_loader )
{
	Agent::SetupProperties( adf_loader );

	std::string sprite_path = adf_loader.GetProperty( "sprite" );
	if ( !sprite_path.empty() )
	{
		sprite_ = new Sprite( vc::GetApp()->CacheImage( sprite_path.c_str() ) );
	}
}

void SpriteAgent::Tick()
{
	Agent::Tick();

	sprite_->position_ = GetPosition();
}

void SpriteAgent::Draw()
{
	sprite_->Draw();
}
