/*
 * variadic function
 * take a variable number of arguments
 */

#include <stdio.h>
#include <stdarg.h>

int sum ( int num_args, ... )
{
    va_list ap;
    int arg, result = 0;
    int i;

    va_start ( ap, num_args );

    for ( i = 0; i < num_args; i++ )
    {
	arg = va_arg ( ap, int );

	result += arg;
    }

    va_end ( ap );

    return result;
}

void show ( const char *fmt, ... )
{
    va_list ap;
    char buf[BUFSIZ];

    va_start ( ap, fmt );

    vsnprintf ( buf, sizeof buf, fmt, ap );

    va_end ( ap );

    printf ( "%s", buf );

    va_start ( ap, fmt );
    vprintf ( fmt, ap );
    va_end ( ap );
}

int main ()
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int rv;

    rv = sum ( 5, a, b, c, d, e );

    show ( "result: %d\n", rv );

    return 0;
}

