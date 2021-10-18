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

#include "../shared.h"
#include "sprite.h"

Sprite::Sprite() : bitmap_( nullptr ), origin_( pl_vecOrigin2 ), position_( pl_vecOrigin2 ), angle( 0 ), _w( 0 ), _h( 0 ),
				   current_frame_( 0 ), frame_count_( 0 ), frame_delay_( 0 ) {}

Sprite::Sprite( ALLEGRO_BITMAP *bitmap ) : bitmap_( bitmap ), origin_( pl_vecOrigin2 ), position_( pl_vecOrigin2 ), angle( 0 ),
										   current_frame_( 0 ), frame_count_( 0 ), frame_delay_( 0 )
{
	frame_w_ = _w = al_get_bitmap_width( bitmap );
	frame_h_ = _h = al_get_bitmap_height( bitmap );
	origin_		  = { _w / 2.0f, _h / 2.0f };
}

Sprite::Sprite( ALLEGRO_BITMAP *bitmap, float x, float y ) : bitmap_( bitmap ), origin_( pl_vecOrigin2 ), position_( { x, y } ), angle( 0 ),
															 current_frame_( 0 ), frame_count_( 0 ), frame_delay_( 0 )
{
	frame_w_ = _w = al_get_bitmap_width( bitmap );
	frame_h_ = _h = al_get_bitmap_height( bitmap );
	origin_		  = { _w / 2.0f, _h / 2.0f };
}

Sprite::~Sprite() = default;

void Sprite::SetBitmap( ALLEGRO_BITMAP *bitmap )
{
	if ( !bitmap )
	{
		Error( "Attempted to pass an invalid bitmap to a sprite!\n" );
	}

	bitmap_ = bitmap;
	_w		= al_get_bitmap_width( bitmap );
	_h		= al_get_bitmap_height( bitmap );
	origin_ = { _w / 2.0f, _h / 2.0f };
}

void Sprite::Draw()
{
	if ( !IsVisible() )
	{
		return;
	}

	al_draw_scaled_rotated_bitmap(
			bitmap_,
			origin_.x, origin_.y,
			position_.x, position_.y,
			1, 1,
			angle,
			0 );
}

bool Sprite::IsVisible()
{
	hei::Vector2 cposition = hei::Vector2( position_ ) - origin_;
	if ( ( ( cposition.x + _w ) < 0 ) || ( cposition.x > DISPLAY_WIDTH ) )
	{
		return false;
	}

	if ( ( ( cposition.y + _h ) < 0 ) || ( cposition.y > DISPLAY_HEIGHT ) )
	{
		return false;
	}

	return true;
}
