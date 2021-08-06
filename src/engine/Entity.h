/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

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
		PL_INLINE PLVector2 GetOrigin() const { return origin; }
		PL_INLINE PLVector2 GetBounds() const { return bounds; }
#endif

		PLVector2 origin{ 0.0f, 0.0f };
		PLVector2 velocity{ 0.0f, 0.0f };
		PLVector2 myRenderBounds{ 0.0f, 0.0f };
		PLVector2 bounds{ 0.0f, 0.0f };

	protected:
		int myHealth{ 100 };
		int myMaxHealth{ 100 };

		bool isVisible{ false };
	};
}// namespace vc
