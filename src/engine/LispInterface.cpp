// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

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

void *l_obj_get(long number) // exten lisp function switches on number
{
	return nullptr;
}

void l_obj_set(long number, void *arg)  // exten lisp function switches on number
{
}

void l_obj_print(long number)  // exten lisp function switches on number
{
}
