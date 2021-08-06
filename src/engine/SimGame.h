/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include <plcore/pl_console.h>

#include <algorithm>
#include <string>
#include <vector>
#include <map>

extern int VC_LOG_MSG;// generic message
extern int VC_LOG_DEB;// debug message (won't be displayed in shipped build)
extern int VC_LOG_WAR;// warning
extern int VC_LOG_ERR;// error (kills application)

#define Print( ... ) \
	PlLogMessage( VC_LOG_MSG, __VA_ARGS__ )
#define Warning( ... ) \
	PlLogMessage( VC_LOG_WAR, __VA_ARGS__ )
#define Error( ... )                         \
	PlLogMessage( VC_LOG_ERR, __VA_ARGS__ ); \
	exit( -1 )
#if defined( DEBUG_BUILD )
#define DebugMsg( ... ) \
	PlLogMessage( VC_LOG_DEB, __VA_ARGS__ )
#else
#define DebugMsg( ... )
#endif

#if defined( DEBUG_BUILD )
#define SgAssert( A, ... ) assert( ( __VA_ARGS__, A ) )
#else
#define SgAssert( A, ... )
#endif

#define IMPLEMENT_SUPER( CLASS ) typedef CLASS SuperClass;

#define FONT_SIZE_LARGE	 50
#define FONT_SIZE_MEDIUM 32
#define FONT_SIZE_SMALL	 8
