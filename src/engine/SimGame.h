/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include <PL/platform_console.h>

enum {
	VC_LOG_MSG, // generic message
	VC_LOG_DEB, // debug message (won't be displayed in shipped build)
	VC_LOG_WAR, // warning
	VC_LOG_ERR, // error (kills application)
};

#define Print( ... )       \
	plLogMessage( VC_LOG_MSG, __VA_ARGS__ )
#define Warning( ... )     \
	plLogMessage( VC_LOG_WAR, __VA_ARGS__ )
#define Error( ... )                         \
	plLogMessage( VC_LOG_ERR, __VA_ARGS__ ); \
	exit( -1 )
#if defined( DEBUG_BUILD )
#   define DebugMsg( ... )    \
	plLogMessage( VC_LOG_DEB, __VA_ARGS__ )
#else
#   define DebugMsg( ... )
#endif

#define IMPLEMENT_SUPER( CLASS ) typedef CLASS SuperClass;

#define FONT_SIZE_LARGE     50
#define FONT_SIZE_MEDIUM    32
#define FONT_SIZE_SMALL     8
