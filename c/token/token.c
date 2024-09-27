/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

int main ( int argc, char **argv, char **envp )
{
    char str[] = "hello nice to meet you";
    char deli[] = " ";
    char *token;

    token = strtok ( str, deli );
    while ( token )
    {
	printf ( "%s\n", token );

	token = strtok ( NULL, deli );
    }

    return 0;
}
