/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

#define VC_LOG   "debug"
#define VC_TITLE "Compton"

#ifdef DEBUG_BUILD
#define WINDOW_TITLE VC_TITLE " [DEBUG]"
#else
#define WINDOW_TITLE VC_TITLE
#endif

#if !defined( NDEBUG )
#define COMPTON_ENABLE_ASSERTS
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
#define SgAssert( A, ... ) assert( ( __VA_ARGS__, A ) )
#else
#define SgAssert( A, ... )
#endif

#define IMPLEMENT_SUPER( CLASS ) typedef CLASS SuperClass;
