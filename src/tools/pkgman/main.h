// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#define Print( ... ) printf( __VA_ARGS__ )
#define Error( ... )       \
	printf( __VA_ARGS__ ); \
	exit( EXIT_FAILURE )

typedef struct PLImage PLImage;

/* pack_image.c */
void PackImage_Write( const char *path, const PLImage *image );
