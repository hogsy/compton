// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "Entity.h"
#include "Serializer.h"
#include "GameMode.h"

ct::Entity::Entity()  = default;
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
	math::Vector2 iso = origin_.ToIso();
	iso.x -= ( int ) camera.position.x;
	iso.y -= ( int ) camera.position.y;

	if ( !ShouldDraw( camera ) )
	{
		render::DrawFilledRectangle( iso.x - bounds_.x, iso.y - bounds_.y, bounds_.x * 2, bounds_.y * 2, hei::Colour( 255, 0, 0, 255 ) );
		return;
	}

	render::DrawFilledRectangle( iso.x - bounds_.x, iso.y - bounds_.y, bounds_.x * 2, bounds_.y * 2, hei::Colour( 0, 255, 0, 255 ) );

	render::drawStats.stats[ render::DrawStats::Type::DRAW_STATS_ENTITY ]++;
}

/**
 * Loading from pre-existing dataset.
 */
void ct::Entity::Deserialize( Serializer *read )
{
	origin_.Deserialize( read );
	bounds_.Deserialize( read );

	velocity_ = read->ReadCoordinate();
}

/**
 * Saving.
 */
void ct::Entity::Serialize( Serializer *write )
{
	origin_.Serialize( write );
	bounds_.Serialize( write );

	write->WriteCoordinate( velocity_ );
}

/**
 * Determine if we should display this particular entity or not.
 */
bool ct::Entity::ShouldDraw( const ct::Camera &camera ) const
{
	math::Vector2 iso = origin_.ToIso();
	iso.x -= ( int ) camera.position.x;
	iso.y -= ( int ) camera.position.y;

	return render::IsVolumeVisible( iso.x - bounds_.x, iso.y - bounds_.y, bounds_.x * 2, bounds_.y * 2 );
}
