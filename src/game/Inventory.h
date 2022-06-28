// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Serializer.h"
#include "Entities/BaseItem.h"

namespace ct
{
	class Inventory : public ISerializable
	{
	public:
		void Deserialize( Serializer *read ) override {}
		void Serialize( Serializer *write ) override {}

		static constexpr unsigned int MAX_SLOTS = 16;
		static constexpr unsigned int MAX_STACK = 16;

	private:
		struct Slot
		{
			BaseItem *items[ MAX_STACK ];
			unsigned int numItems{ 0 };
		};
		Slot items_[ MAX_SLOTS ];
	};
}// namespace ct
