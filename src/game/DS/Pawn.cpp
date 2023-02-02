// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Game.h"
#include "../Entities/BaseCharacter.h"

namespace ct::game::ds
{
	class Pawn : public BaseCharacter
	{
		DECLARE_ENTITY_CLASS( Pawn, BaseCharacter )

	public:
		void Precache() override;
		void Spawn() override;
		void Tick() override;

	private:
		static const SpriteAnimation *idleAnimation;
		static const SpriteAnimation *runAnimations[ ct::game::MAX_DIRECTIONS ];

		static constexpr const char *ANIMATION_PATH = "sprites/pawns/pawn_basic.ani";
		static constexpr const char *SPRITE_PATH    = "sprites/pawns/pawn_basic.spr";
	};

	const SpriteAnimation *Pawn::idleAnimation = nullptr;
	const SpriteAnimation *Pawn::runAnimations[];

	Pawn::Pawn()
	{
		bounds_.x = 10;
		bounds_.y = 8;
	}

	Pawn::~Pawn() {}

	void Pawn::Precache()
	{
		SuperClass::Precache();

		if ( spriteManager->GetSpriteSheet( SPRITE_PATH ) == nullptr )
			return;

		if ( !CacheAnimationSet( ANIMATION_PATH ) )
			return;

		idleAnimation = SpriteAnimator::GetAnimation( ANIMATION_PATH, "pawn_idle" );
		if ( runAnimations[ 0 ] == nullptr )
		{
			for ( unsigned int i = 0; i < MAX_DIRECTIONS; ++i )
			{
				std::string name   = "pawn_run_" + std::string( DIRECTIONS[ i ] );
				runAnimations[ i ] = SpriteAnimator::GetAnimation( ANIMATION_PATH, name );
				runAnimations[ i ]->playbackSpeed;
			}
		}
	}

	void Pawn::Spawn()
	{
		SuperClass::Spawn();

		SetAnimation( idleAnimation );
	}

	void Pawn::Tick()
	{
		math::Vector2 oldOrigin = origin_;

		SuperClass::Tick();

		if ( IsMoving() )
		{
			if ( origin_.x > oldOrigin.x )
				SetAnimation( runAnimations[ DIR_WEST ] );
			else if ( origin_.x < oldOrigin.x )
				SetAnimation( runAnimations[ DIR_EAST ] );
			else if ( origin_.y > oldOrigin.y )
				SetAnimation( runAnimations[ DIR_SOUTH ] );
			else if ( origin_.y < oldOrigin.y )
				SetAnimation( runAnimations[ DIR_NORTH ] );
		}
		else
			SetAnimation( idleAnimation );

	}
}// namespace ct::game::ds

REGISTER_ENTITY( Pawn, ct::game::ds::Pawn )
