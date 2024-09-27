/*
 * tortoise test
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>

#include "zut.h"
//#include "dut.h"
#include "zlogger.h"
#include "zinet.h"

#include "log.h"
#include "tortoise.h"


#undef TT_SEND
#undef TT_COMM
#undef TT_COMM_TO

#define TT_SEND(fmt, arg...)			\
    SYSTEM ( "./tortoise '" fmt "'", ##arg )

#define TT_COMM(fmt, arg...)			\
    SYSTEM ( "./tortoise -m comm " TORTOISE_DELIMITER " '" fmt "'", ##arg )

#define TT_COMM_TO(TO, fmt, arg...)		\
    SYSTEM ( "./tortoise -m comm -t " TOSTR(TO) TORTOISE_DELIMITER " '" fmt "'", ##arg )

#define TT_READ(V, fmt, arg...)			\
    SHELL_READ ( V, "./tortoise -m comm -r " TORTOISE_DELIMITER " '" fmt "'", ##arg )

/*
 *
 */
int
main ( int argc, char **argv, char **envp )
{
    int rv;
    char buf[128];

    printf("Rand=%d\n", Rand());
#if 0
    rv = TT_SEND (  "cat Makefile.test | wc -l" );
    printf ( "rv(%d)\n", rv );

    TT_SEND (  "cat Makefile.test | wc -l" );

    TT_COMM (  "cat Makefile.test | wc -l" );

    TT_READ ( buf, "cat Makefile.test | wc -l" );
    printf ( "result: '%s'\n", buf );

    TT_COMM (  "cat Makefile.test | wc -l" );

    TT_READ ( buf, "cat Makefile.test | wc -l" );
#endif


    return 0;
}

