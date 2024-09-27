/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>

struct options {
    char *message;
    char *alarm;
    char *alm_on;
    int server_mode;
};

/*
 *
 */
int
opt_parser ( int argc, char **argv, struct options *p )
{
    const char *optstring = "m:a:s";
    int c;	/* option */

    while ( (c = getopt(argc, argv, optstring)) != -1 )
    {
	switch ( c )
	{
	case 'm':
	    p->message = optarg;
	    break;

	case 'a':
	    p->alarm = optarg;
	    if ( optind < argc && argv[optind][0] != '-' )
		p->alm_on = argv[optind++];
	    break;

	case 's':
	    p->server_mode = true;
	    break;
	}
    }

    return 0;
}

int main ( int argc, char **argv, char **envp )
{
    struct options opt;

    memset ( &opt, 0, sizeof opt );

    opt_parser ( argc, argv, &opt );

    printf ( "argc       : %d\n", argc );
    printf ( "message    : %s\n", opt.message );
    printf ( "alarm      : %s\n", opt.alarm );
    printf ( "alarm on   : %s\n", opt.alm_on );
    printf ( "server mode: %d\n", opt.server_mode );

    return 0;
}

#define USAGE_NEWLINE	"\n           "

void
usage ( void )
{
    fprintf ( stderr,
	"Usage: nms"
	" [-"
		"mas"
	"]"
	" [-m MESSAGE]"
	" [-a ALARM on|off]"
	USAGE_NEWLINE
	" MESSAGE: report, alarm"
	" ALARM: cpu, memory, wwan, eth0"
	"\n"
    );

    exit ( 2 );
}

