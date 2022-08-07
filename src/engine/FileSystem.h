// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <plcore/pl_filesystem.h>

namespace ct::fs
{
	char *LoadFileIntoBuffer( const char *path, unsigned int *length );

	const char *GetDataLocation();
}// namespace vc::fs
