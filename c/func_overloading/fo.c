/*
 * supported in GCC since version 4.9
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addi ( int x, int y )
{
    return x + y;
}

char *adds ( char *x, char *y )
{
    char *z = malloc(strlen(x) + strlen(y) + 1);

    strcpy ( z, x );
    strcat ( z, y );

    return z;
}

#define add(x, y)	\
    _Generic(x, int: addi, char *: adds)(x, y)

int main ( void )
{
    int x = 5, y = 8;
    char *c = "Linux", *d = "system";

    printf ( "The sum is: %d\n", add(x, y) );
    printf ( "The string is: %s\n", add(c, d) );

    return 0;
}

