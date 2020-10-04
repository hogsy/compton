/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "EntityManager.h"

namespace vc {
	class Serializer;
	class Entity {
	public:
		Entity();
		~Entity();

		virtual void Spawn();

		virtual void Tick();
		virtual void Draw();

		virtual void Deserialize( Serializer *read );
		virtual void Serialize( Serializer *write );

		PL_INLINE PLVector2 GetOrigin() const { return origin; }
		PL_INLINE PLVector2 GetBounds() const { return bounds; }

	protected:
		bool isVisible{ false };

		PLVector2 origin{ 0.0f, 0.0f }, bounds{ 0.0f, 0.0f };
	};
}
