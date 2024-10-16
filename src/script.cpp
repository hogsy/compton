// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "lisp.h"

#include "app.h"
#include "script.h"

bool CompileFromFile( const std::string &path )
{
	return CompileFromString( "(load \"" + path + "\")\n" );
}

bool CompileFromString( const std::string &eval )
{
	char const *cs = eval.c_str();
	if ( !LObject::Compile( cs )->Eval() )
	{
		Warning( "Failed to compile \"%s\"\n", eval.c_str() );
		return false;
	}

	LSpace::Tmp.Clear();

	return true;
}

///////////////////////////////////////////////////
// LISP Callbacks
///////////////////////////////////////////////////

void clisp_init()
{
}

void *l_caller( long number, void *args )
{
	return nullptr;
}

long c_caller( long number, void *args )
{
	return 0;
}

void *l_obj_get( long number )// exten lisp function switches on number
{
	return nullptr;
}

void l_obj_set( long number, void *arg )// exten lisp function switches on number
{
}

void l_obj_print( long number )// exten lisp function switches on number
{
}
