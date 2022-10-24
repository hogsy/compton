// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Entity.h"
#include "engine/Sprite.h"

namespace ct
{
	enum
	{
		ITEM_TYPE_INVALID,
		ITEM_TYPE_RESOURCE,
		ITEM_TYPE_WEAPON,
		ITEM_TYPE_TOOL,
		ITEM_TYPE_CUSTOM,

		MAX_ITEM_TYPES
	};

	class BaseItem : public Entity
	{
		DECLARE_ENTITY_CLASS( BaseItem, Entity )

	public:
		virtual const Sprite *GetIcon() { return nullptr; }

		virtual void Use( Entity *user ) {}
		virtual void Drop() {}

	private:
		unsigned int itemType_{ ITEM_TYPE_INVALID };
	};
}// namespace ct
