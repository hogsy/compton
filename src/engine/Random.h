// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

namespace vc
{
	namespace random
	{
		int         GenerateRandomInteger( int min, int max );
		const char *GenerateRandomName( char *buffer, size_t size );
	}// namespace random
}// namespace vc
