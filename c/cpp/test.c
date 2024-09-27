/*
 * gcc --save-temps test.c
 */

#include <stdio.h>

#include "config.h"

#ifdef HAVE_DSW5306
#include "test.h"
#endif

#ifndef TEST_STR
#define TEST_STR	"UNDEF"
#endif

int
main ( int argc, char **argv, char **envp )
{
    printf ( "%s\n", TEST_STR );

    return 0;
}

