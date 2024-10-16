// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "entity_manager.h"

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
	int health{ 100 };
	int maxHealth{ 100 };

	bool isVisible{ false };
};

#define DECLARE_ENTITY_CLASS( CLASSNAME, BASECLASS ) \
	IMPLEMENT_SUPER( BASECLASS )                     \
public:                                              \
	CLASSNAME();                                     \
	~CLASSNAME();                                    \
	virtual const char *GetClassIdentifier() const override { return #CLASSNAME; }
