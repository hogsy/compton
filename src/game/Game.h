// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "engine/Compton.h"
#include "engine/Random.h"
#include "engine/Serializer.h"

#include "Utility.h"
#include "DSGameMode.h"

namespace ct::game
{
	enum Direction
	{
		DIR_NORTH,
		DIR_EAST,
		DIR_SOUTH,
		DIR_WEST,

		MAX_DIRECTIONS
	};

	static constexpr unsigned int MAX_SPRITE_DIR = MAX_DIRECTIONS;// N, NE, E, SE, S - then mirror
}// namespace ct::game
