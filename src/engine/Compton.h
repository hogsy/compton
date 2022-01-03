// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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

#define VC_LOG      "debug"
#define VC_TITLE    "Compton"
#define VC_CODENAME "SAUSAGE"

#ifdef DEBUG_BUILD
#	define WINDOW_TITLE VC_TITLE " [DEBUG]"
#else
#	define WINDOW_TITLE VC_TITLE
#endif

#if !defined( NDEBUG )
#	define COMPTON_ENABLE_ASSERTS
#endif

#define Print( ... ) \
	PlLogMessage( VC_LOG_MSG, __VA_ARGS__ )
#define Warning( ... ) \
	PlLogMessage( VC_LOG_WAR, __VA_ARGS__ )
#define Error( ... )                             \
	{                                            \
		PlLogMessage( VC_LOG_ERR, __VA_ARGS__ ); \
		abort();                                 \
	}
#define DebugMsg( ... ) \
	PlLogMessage( VC_LOG_DEB, __VA_ARGS__ )

#if defined( COMPTON_ENABLE_ASSERTS )
#	define SgAssert( A, ... ) assert( ( __VA_ARGS__, A ) )
#else
#	define SgAssert( A, ... )
#endif

#define IMPLEMENT_SUPER( CLASS ) typedef CLASS SuperClass;
