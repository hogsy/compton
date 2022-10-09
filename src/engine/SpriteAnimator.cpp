// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_parse.h>

#include "SpriteAnimator.h"
#include "SpriteSheet.h"

std::map< std::string, std::map< std::string, ct::SpriteAnimation > >
		ct::SpriteAnimator::animationSets_;

bool ct::SpriteAnimator::CacheAnimationSet( const char *path )
{
	// It's already been cached...
	auto i = animationSets_.find( path );
	if ( i != animationSets_.end() )
		return true;

	unsigned int length;
	char *buffer = fs::LoadFileIntoBuffer( path, &length );
	if ( buffer == nullptr )
		return false;

	auto animations = ParseFile( buffer );
	bool status = !animations.empty();
	if ( status )
	{
		animationSets_.emplace( path, animations );
		Print( "Cached animation, \"%s\"\n", path );
	}

	delete[] buffer;

	return status;
}

std::map< std::string, ct::SpriteAnimation > ct::SpriteAnimator::ParseFile( const char *buffer )
{
	// todo: rewrite this (and the sprite equivalent) to parse per-line instead

	std::map< std::string, SpriteAnimation > animations;

	const SpriteSheet *spriteSheet = nullptr;
	SpriteAnimation animation;
	const char *p = buffer;
	while ( true )
	{
		if ( *p == '\0' )
			return animations;

		static constexpr unsigned int MAX_TOKEN = 256;
		char token[ MAX_TOKEN ];
		if ( PlParseToken( &p, token, MAX_TOKEN ) == nullptr )
			break;

		if ( *token == ';' || PlIsEndOfLine( token ) )
		{
			PlSkipLine( &p );
			continue;
		}
		else if ( pl_strcasecmp( token, "$sheet" ) == 0 )
		{
			const char *path = PlParseEnclosedString( &p, token, MAX_TOKEN );
			if ( path == nullptr )
			{
				Warning( "Invalid path for sprite sheet!\n" );
				break;
			}

			spriteSheet = ct::spriteManager->GetSpriteSheet( path );
			if ( spriteSheet == nullptr )
			{
				Warning( "Failed to fetch specified sprite sheet: %s\n", path );
				break;
			}

			PlSkipLine( &p );
			continue;
		}
		else if ( pl_strcasecmp( token, "$loop" ) == 0 )
		{
			animation.loop = true;
			PlSkipLine( &p );
			continue;
		}
		else if ( pl_strcasecmp( token, "$speed" ) == 0 )
		{
			animation.playbackSpeed = PlParseInteger( &p, nullptr );
			PlSkipLine( &p );
			continue;
		}
		else if ( pl_strcasecmp( token, "$frame" ) == 0 )
		{
			const char *frameName = PlParseEnclosedString( &p, token, MAX_TOKEN );
			if ( frameName == nullptr )
			{
				Warning( "Invalid name for sprite!\n" );
				break;
			}

			SpriteAnimation::Frame frame;
			frame.sprite = spriteSheet->LookupElement( frameName );
			if ( frame.sprite == nullptr )
			{
				Warning( "Failed to fetch sprite '%s' from sprite sheet!\n", frameName );
				break;
			}

			frame.mirror = ( PlParseInteger( &p, nullptr ) == 1 );
			frame.origin.x = PlParseFloat( &p, nullptr );
			frame.origin.y = PlParseFloat( &p, nullptr );

			animation.frames.push_back( frame );
		}
		else if ( pl_strcasecmp( token, "$push" ) == 0 )
		{
			const char *name = PlParseEnclosedString( &p, token, MAX_TOKEN );
			if ( name == nullptr )
			{
				Warning( "Invalid name for animation!\n" );
				break;
			}

			animations.emplace( name, animation );
			PL_ZERO_( animation );

			Print( "Pushed anim: %s\n", name );

			PlSkipLine( &p );
			continue;
		}

		PlSkipLine( &p );
	}

	return animations;
}

void ct::SpriteAnimator::SetAnimation( const SpriteAnimation *animation )
{
	frame_ = 0;
	animation_ = animation;
}

void ct::SpriteAnimator::Tick()
{
	if ( animation_ == nullptr || animation_->frames.empty() )
		return;

	if ( nextFrameTime_ > ct::GetApp()->GetNumOfTicks() )
		return;

	nextFrameTime_ = ct::GetApp()->GetNumOfTicks() + animation_->playbackSpeed;
	frame_++;
	if ( frame_ >= animation_->frames.size() )
	{
		if ( !animation_->loop )
		{
			frame_ = animation_->frames.size() - 1;
			return;
		}

		frame_ = 0;
		return;
	}
}

void ct::SpriteAnimator::Draw( int x, int y )
{
	if ( animation_ == nullptr )
		return;

	const SpriteAnimation::Frame *frame = GetCurrentFrame();
	if ( frame->sprite == nullptr )
		return;

	x -= frame->origin.x;
	y -= frame->origin.y;

	frame->sprite->Draw( x, y, true, frame->mirror );
}
