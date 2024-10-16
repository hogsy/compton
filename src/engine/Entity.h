// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <cfloat>

#include "EntityManager.h"

#define DECLARE_ENTITY_CLASS( CLASSNAME, BASECLASS )        \
	IMPLEMENT_SUPER( BASECLASS )                            \
public:                                                     \
	CLASSNAME();                                            \
	~CLASSNAME();                                           \
	virtual const char *GetClassIdentifier() const override \
	{                                                       \
		return #CLASSNAME;                                  \
	}

namespace ct
{
	class Serializer;
	class Entity
	{
	public:
		Entity();
		~Entity();

		[[nodiscard]] virtual const char *GetClassIdentifier() const { return "Entity"; }

		virtual void Spawn();
		virtual void Precache() {}

		virtual void Tick();
		virtual void Draw( const Camera &camera );

		virtual void Deserialize( Serializer *read );
		virtual void Serialize( Serializer *write );

		[[nodiscard]] virtual bool ShouldDraw( const Camera &camera ) const;

		hei::Vector2 velocity_;

		math::Vector2 origin_, oldOrigin_;
		math::Vector2 offset_;
		math::Vector2 bounds_{ 20, 20 };

		int z_{ 0 };

	protected:
		int health_{ 100 };
		int maxHealth_{ 100 };

		bool isVisible{ false };
	};
}// namespace ct
