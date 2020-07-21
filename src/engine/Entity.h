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

		virtual void Tick();
		virtual void Draw();

		virtual void Deserialize( Serializer *read );
		virtual void Serialize( Serializer *write );

	protected:
		bool isVisible{ false };

		PLVector2 origin;
	};
}
