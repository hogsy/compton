// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Random.h"
#include "Serializer.h"
#include "Utility.h"
#include "GameMode.h"

namespace vc::game
{
	enum Direction
	{
		DIR_NORTH,
		DIR_NORTH_EAST,
		DIR_EAST,
		DIR_SOUTH_EAST,
		DIR_SOUTH,
		DIR_SOUTH_WEST,
		DIR_WEST,
		DIR_NORTH_WEST,
	};

	static constexpr unsigned int MAX_SPRITE_DIR = 4;// N, NE, E, SE, S - then mirror
}// namespace vc::game
