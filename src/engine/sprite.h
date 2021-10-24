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

#pragma once

struct ALLEGRO_BITMAP;

class Sprite
{
public:
	Sprite();
	explicit Sprite( ALLEGRO_BITMAP *bitmap );
	Sprite( ALLEGRO_BITMAP *bitmap, float x, float y );
	virtual ~Sprite();

	ALLEGRO_BITMAP *GetBitmap() { return bitmap_; }
	virtual void	SetBitmap( ALLEGRO_BITMAP *bitmap );

	virtual void Draw();

	bool IsVisible();

	PLVector2 position_;
	PLVector2 origin_;
	float	  angle;

protected:
private:
	ALLEGRO_BITMAP *bitmap_;

	unsigned int frame_count_, current_frame_;
	int			 frame_w_, frame_h_;
	double		 frame_delay_;

	int _w, _h;
};