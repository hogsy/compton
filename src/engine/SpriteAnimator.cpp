// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include <plcore/pl_parse.h>

#include "SpriteAnimator.h"
#include "SpriteSheet.h"

bool vc::SpriteAnimator::LoadFile( const char *path )
{
	// It's already been cached...
	auto i = cachedAnimations_.find( path );
	if ( i != cachedAnimations_.end() )
		return true;

	unsigned int length;
	char *buffer = fs::LoadFileIntoBuffer( path, &length );
	if ( buffer == nullptr )
		return false;

	bool status = ParseFile( buffer );
	if ( status )
		cachedAnimations_.emplace( path, animations_ );

	delete[] buffer;

	return status;
}

bool vc::SpriteAnimator::ParseFile( const char *buffer )
{
	// todo: rewrite this (and the sprite equivalent) to parse per-line instead

	const SpriteSheet *spriteSheet = nullptr;
	SpriteAnimation animation;
	const char *p = buffer;
	while ( true )
	{
		if ( *p == '\0' )
		{
			return true;
		}

		static constexpr unsigned int MAX_TOKEN = 256;
		char token[ MAX_TOKEN ];
		if ( PlParseToken( &p, token, MAX_TOKEN ) == nullptr )
			break;

		if ( *token == ';' || PlIsEndOfLine( p ) )
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

			spriteSheet = vc::spriteManager->GetSpriteSheet( path );
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
			animation.playbackSpeed = PlParseFloat( &p, nullptr );
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

			animations_.emplace( name, animation );
			PL_ZERO_( animation );

			PlSkipLine( &p );
			continue;
		}

		PlSkipLine( &p );
	}

	return false;
}

void vc::SpriteAnimator::SetAnimation( const char *name )
{
	auto i = animations_.find( name );
	if ( i == animations_.end() )
	{
		Warning( "Failed to find animation: %s\n", name );
		return;
	}

	currentAnimation_ = &i->second;
}

void vc::SpriteAnimator::Tick()
{
}

void vc::SpriteAnimator::Draw( const hei::Vector2 &position )
{
	if ( currentAnimation_ == nullptr )
		return;

	const SpriteAnimation::Frame *frame = &currentAnimation_->frames[ currentAnimation_->currentFrame ];
	if ( frame->sprite == nullptr )
		return;

	int x = position.x + frame->origin.x;
	int y = position.y + frame->origin.y;

	frame->sprite->Draw( x, y, true, frame->mirror );
}
