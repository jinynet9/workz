#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int sum (int, int );

int main ( int argc, char **argv )
{
    int a, b, n;

    a = atoi ( argv[1] );
    b = atoi ( argv[2] );

    n = sum ( a, b );

    printf ( "sum(%d, %d) = %d\n", a, b, n );

    return 0;
}

#if 0
$ ln -s libmystuff.so.1.0.1 libmystuff.so.1

$ LD_LIBRARY_PATH=./ ./x 1 2
sum(1, 2) = 3
#endif

