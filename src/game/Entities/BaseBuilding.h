// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Entity.h"
#include "Serializer.h"

namespace ct
{
	class BaseBuilding : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseBuilding, Entity )

	public:
		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		inline bool IsBuildingFull() const
		{
			return ( occupants_.size() >= maxOccupants_ );
		}

		inline unsigned int GetMaxOccupants() const
		{
			return maxOccupants_;
		}

		inline unsigned int GetNumOccupants() const
		{
			return occupants_.size();
		}

		void Draw( const Camera &camera ) override;

	protected:
		uint8_t maxConstructionAmount_{ 100 };// Amount to reach before building is finalised

	private:
		uint8_t constructionAmount_{ 0 };

		uint8_t maxOccupants_{ 0 };
		std::vector< Entity * > occupants_;
	};
}// namespace vc
