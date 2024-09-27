/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>


int write_main ( int argc, char **argv );
int read_main ( int argc, char **argv );
int main ( int argc, char **argv, char **envp )
{
    read_main ( argc, argv );
}

int read_main ( int argc, char **argv )
{
    FILE *fp;
    char buf[256];
    char name[256], value[256];
    int rv;

    if ( argc < 2 )
    {
	printf ( "usage: %s FILE\n", argv[0] );
	return -1;
    }

    fp = fopen ( argv[1], "r" );
    if ( ! fp )
	return -1;

    while ( fgets(buf, sizeof buf, fp) )
    {
	printf ( "read:[%s]\n", buf );

	rv = sscanf ( buf, "%s = %s", name, value );
	if ( rv != 2 )
	{
	    printf ( "invalid string\n" );
	    continue;
	}

	printf ( "name[%s], value[%s]\n", name, value );
    }

    fclose ( fp );

    return 0;
}

#define CONF_WRITE(fp, name, value)				\
    do {							\
	char buf[256];						\
	snprintf ( buf, sizeof buf, "%s = %s\n", name, value );	\
	fputs ( buf, fp );					\
    } while (0)

int write_main ( int argc, char **argv )
{
    FILE *fp;

    if ( argc < 2 )
    {
	printf ( "usage: %s FILE\n", argv[0] );
	return -1;
    }

    fp = fopen ( argv[1], "w" );
    if ( ! fp )
	return -1;

    CONF_WRITE ( fp, "enable", "1" );
    CONF_WRITE ( fp, "interval", "60" );

//    fwrite ( buf, 1, strlen(buf), fp );

    fclose ( fp );

    return 0;
}

