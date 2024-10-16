// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine_entity.h"

namespace vc
{
	class Boid : public Entity
	{
		DECLARE_ENTITY_CLASS( Boid, Entity )

	public:
		enum class Type
		{
			SMALL_BIRD,
			BIG_BIRD,

			NUM_TYPES
		};

		void Spawn() override;
		void Draw( const Camera &camera ) override;
		void Tick() override;

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

	private:
		static constexpr unsigned int NUM_FRAMES[ ( unsigned int ) Type::NUM_TYPES ] =
		        {
		                6,// small bird
		                8 // big bird
		        };
		ImageManager::Sprite *sprites_[ ( unsigned int ) Type::NUM_TYPES ];

		Type type_{ Type::SMALL_BIRD };
	};
}// namespace vc
