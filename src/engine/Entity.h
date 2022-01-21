// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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
		virtual void Precache() {}

		virtual void Tick();
		virtual void Draw( const Camera &camera );

		virtual void Deserialize( Serializer *read );
		virtual void Serialize( Serializer *write );

		virtual bool ShouldDraw( const Camera &camera ) const;

		hei::Vector2 velocity{ 0.0f, 0.0f };
		hei::Vector2 bounds{ 0.0f, 0.0f };
		hei::Vector2 origin{ 0.0f, 0.0f };

	protected:
		int myHealth{ 100 };
		int myMaxHealth{ 100 };

		bool isVisible{ false };
	};
}// namespace vc
