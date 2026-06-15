/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>

void
usage ( void );

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
opts_parser ( int argc, char **argv, struct options *p )
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

	default:
	    fprintf(stderr, "Invalid option: %s\n", argv[optind-1]);
	    usage();
	}
    }

    return 0;
}

int main ( int argc, char **argv, char **envp )
{
    struct options opts;

    memset ( &opts, 0, sizeof opts );

    opts_parser ( argc, argv, &opts );

    printf ( "argc       : %d\n", argc );
    printf ( "message    : %s\n", opts.message );
    printf ( "alarm      : %s\n", opts.alarm );
    printf ( "alarm on   : %s\n", opts.alm_on );
    printf ( "server mode: %d\n", opts.server_mode );

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

