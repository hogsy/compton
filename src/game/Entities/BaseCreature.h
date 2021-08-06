/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"
#include "AIBrain.h"

namespace vc
{
	class BaseCreature : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseCreature, Entity )

	public:
		enum class Sex
		{
			MALE,
			FEMALE,
			INTERSEX,

			MAX_SEXES
		};

		virtual void Spawn() override;

		bool CanBreed( BaseCreature *other );

		virtual void Deserialize( Serializer *read ) override;
		virtual void Serialize( Serializer *write ) override;

		virtual void Draw( const Camera &camera ) override;
		virtual void Tick() override;

	protected:
		Sex mySex{ Sex::INTERSEX };

		AIBrain myBrain;

		unsigned int age{ 0 };
		unsigned int maxAge{ 100 };
		unsigned int generation{ 0 };

		bool		 isPregnant{ false };
		unsigned int timePregnant{ 0 };

		int stamina{ 100 };
		int maxStamina{ 100 };

		int myHunger{ 0 };
		int myThirst{ 0 };

		unsigned int experience{ 0 };
		unsigned int maxExperience{ 100 };
		unsigned int currentLevel{ 0 };
	};
}// namespace vc
