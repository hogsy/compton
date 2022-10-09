// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "Serializer.h"
#include "GameMode.h"

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
#if 0// TODO move into DSBaseEntity class
	// Check if we've moved since last tick
	if ( origin_ != oldOrigin_ )
	{
		DSGameMode *gameMode = ( DSGameMode * ) GetApp()->GetGameMode();
		if ( gameMode != nullptr && gameMode->GetWorld()->GetQuadrant( origin_.x, origin_.y ) == nullptr )
		{

		}

		oldOrigin_ = origin_;
	}
#endif
}

void ct::Entity::Draw( const Camera &camera )
{
	int x, y;
	render::TransformToIso( ( int ) origin_.x, ( int ) origin_.y, &x, &y );

	x -= ( int ) camera.position.x;
	y -= ( int ) camera.position.y;

	render::DrawPixel( x, y, hei::Colour( 255, 0, 0, 255 ) );
	render::DrawPixel( x + ( int ) offset_.x, y + ( int ) offset_.y, hei::Colour( 0, 255, 0, 255 ) );

	render::DrawPixel( x - bounds_.x, y, hei::Colour( 0, 0, 255, 255 ) );
	render::DrawPixel( x + bounds_.x, y, hei::Colour( 0, 0, 255, 255 ) );
	render::DrawPixel( x, y - bounds_.y, hei::Colour( 0, 0, 255, 255 ) );
	render::DrawPixel( x, y + bounds_.y, hei::Colour( 0, 0, 255, 255 ) );
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
	int x, y;
	render::TransformToIso( ( int ) origin_.x, ( int ) origin_.y, &x, &y );

	if ( x - bounds_.x > camera.position.x + DISPLAY_WIDTH )
		return false;
	else if ( y - bounds_.y > camera.position.y + DISPLAY_HEIGHT )
		return false;
	else if ( x + bounds_.x < camera.position.x - DISPLAY_WIDTH )
		return false;
	else if ( y + bounds_.y < camera.position.y - DISPLAY_HEIGHT )
		return false;

	return true;
}
