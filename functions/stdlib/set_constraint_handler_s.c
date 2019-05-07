/* set_constraint_handler_s( constraint_handler_t )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>

#ifndef REGTEST

static void constraint_handler( const char * _PDCLIB_restrict msg, void * _PDCLIB_restrict ptr, errno_t errno )
{
    abort();
}

static constraint_handler_t _PDCLIB_constraint_handler = constraint_handler;

constraint_handler_t set_constraint_handler_s( constraint_handler_t handler )
{
    constraint_handler_t previous = _PDCLIB_constraint_handler;

    if ( handler == NULL )
    {
        _PDCLIB_constraint_handler = constraint_handler;
    }
    else
    {
        _PDCLIB_constraint_handler = handler;
    }

    return previous;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

#ifndef REGTEST
static void new_constraint_handler( const char * _PDCLIB_restrict msg, void * _PDCLIB_restrict ptr, errno_t errno )
{
    abort();
}
#endif

int main( void )
{
#ifndef REGTEST
    TESTCASE( _PDCLIB_constraint_handler == constraint_handler );
    TESTCASE( set_constraint_handler_s( new_constraint_handler ) == constraint_handler );
    TESTCASE( _PDCLIB_constraint_handler == new_constraint_handler );
    TESTCASE( set_constraint_handler_s( NULL ) == new_constraint_handler );
    TESTCASE( _PDCLIB_constraint_handler == constraint_handler );
#endif
    return TEST_RESULTS;
}

#endif
