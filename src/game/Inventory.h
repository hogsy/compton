// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Serializer.h"

namespace ct
{
	class Inventory : public ISerializable
	{
	public:
		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		
	};
}