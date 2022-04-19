// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "Serializer.h"

ct::Entity::Entity() = default;
ct::Entity::~Entity() = default;

/**
 * Called if the entity is spawning anew.
 */
void ct::Entity::Spawn()
{
}

void ct::Entity::Tick()
{
}

void ct::Entity::Draw( const Camera &camera )
{
	hei::Vector2 pos = origin_ - camera.position;
	render::DrawPixel( pos.x, pos.y, hei::Colour( 255, 0, 0, 255 ) );
	hei::Vector2 off = pos + offset_;
	render::DrawPixel( off.x, off.y, hei::Colour( 0, 255, 0, 255 ) );

	hei::Vector2 b = pos;
	b.x = pos.x - bounds_.x;
	render::DrawPixel( b.x, b.y, hei::Colour( 0, 0, 255, 255 ) );
	b.x = pos.x + bounds_.x;
	render::DrawPixel( b.x, b.y, hei::Colour( 0, 0, 255, 255 ) );
	b.x = pos.x;
	b.y = pos.y - bounds_.y;
	render::DrawPixel( b.x, b.y, hei::Colour( 0, 0, 255, 255 ) );
	b.y = pos.y + bounds_.y;
	render::DrawPixel( b.x, b.y, hei::Colour( 0, 0, 255, 255 ) );
}

/**
 * Loading from pre-existing dataset.
 */
void ct::Entity::Deserialize( Serializer *read )
{
	origin_ = read->ReadCoordinate();
	velocity_ = read->ReadCoordinate();
	bounds_ = read->ReadCoordinate();
}

/**
 * Saving.
 */
void ct::Entity::Serialize( Serializer *write )
{
	write->WriteCoordinate( origin_ );
	write->WriteCoordinate( velocity_ );
	write->WriteCoordinate( bounds_ );
}

/**
 * Determine if we should display this particular entity or not.
 */
bool ct::Entity::ShouldDraw( const ct::Camera &camera ) const
{
	if ( origin_.x - bounds_.x > camera.position.x + DISPLAY_WIDTH )
	{
		return false;
	}
	else if ( origin_.y - bounds_.y > camera.position.y + DISPLAY_HEIGHT )
	{
		return false;
	}
	else if ( origin_.x + bounds_.x < camera.position.x - DISPLAY_WIDTH )
	{
		return false;
	}
	else if ( origin_.y + bounds_.y < camera.position.y - DISPLAY_HEIGHT )
	{
		return false;
	}

	return true;
}
