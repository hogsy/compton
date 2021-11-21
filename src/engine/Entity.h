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

#include "EntityManager.h"

#define DECLARE_ENTITY_CLASS( CLASSNAME, BASECLASS ) \
	IMPLEMENT_SUPER( BASECLASS )                     \
public:                                              \
	CLASSNAME();                                     \
	~CLASSNAME();                                    \
	virtual const char *GetClassIdentifier() const override { return #CLASSNAME; }

namespace vc
{
	class Serializer;
	class Entity
	{
	public:
		Entity();
		~Entity();

		virtual const char *GetClassIdentifier() const { return "Entity"; }

		virtual void Spawn();

		virtual void Tick();
		virtual void Draw( const Camera &camera );

		virtual void Deserialize( Serializer *read );
		virtual void Serialize( Serializer *write );

		virtual bool ShouldDraw( const Camera &camera ) const;

#if 0
		inline PLVector2 GetBounds() const { return bounds; }
#endif

		hei::Vector2 velocity{ 0.0f, 0.0f };
		hei::Vector2 bounds{ 0.0f, 0.0f };
		hei::Vector2 origin{ 0.0f, 0.0f };

	protected:
		int myHealth{ 100 };
		int myMaxHealth{ 100 };

		bool isVisible{ false };
	};
}// namespace vc
