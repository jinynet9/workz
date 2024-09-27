/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#define DIM(v)			( sizeof(v)/sizeof(v[0]) )

#define FLAG_SET(v,n)          do { (v) |= (n); } while (0)
#define FLAG_CLR(v,n)          do { (v) &= ~(n); } while (0)
#define FLAG_ISSET(v,n)        ( (v) & (n) ? 1 : 0 )


int si_main ( int argc, char **argv, char **envp );
int redirect_main ( int argc, char **argv, char **envp );
int select_main ( int argc, char **argv, char **envp );

#define HOURS	(60 * 60)
#define DAYS	(24 * HOURS)

int main ( int argc, char **argv, char **envp )
{
#if 0
    struct sysinfo sinfo;

    sysinfo ( &sinfo );

    printf ( "uptime: %ld\n", sinfo.uptime );
    printf ( "%d days\n", sinfo.uptime / DAYS );
#endif

    select_main ( argc, argv, envp );

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// select timeout
//

int
select_main ( int argc, char **argv, char **envp )
{
    struct timeval tv = { 0, 0 };
    int loop = 0;

    while ( 1 )
    {
	select ( 0, NULL, NULL, NULL, &tv );

	printf ( "loop %d\n", ++loop );

	/* 100 ms */
	tv.tv_usec = 100 * 1000;

	continue;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// 
//

#if 0
./a.out << EOF
hello world
EOF

read: 12
#endif
int
redirect_main ( int argc, char **argv, char **envp )
{
    char buffer[BUFSIZ];
    int rv;

    rv = fread ( buffer, sizeof(char), BUFSIZ, stdin );
    fprintf ( stderr, "\nread: %d\n", rv );

    exit (0);

    return 0;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
//
// 
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>

#include "../zlib/zut.h"

static int
_proc_traverse ( void )
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir ( "/proc" );
    if ( ! dir )
	return -1;

    while ( (ent = readdir(dir)) )
    {
	printf ( "%s\n", ent->d_name );
    }

    closedir ( dir );

    return 0;
}

static int
_get_myname ( char *buf, int buflen )
{
    pid_t pid = getpid();
    char cmdline[512];
    char *p;

    SHELL_READ ( cmdline, "cat /proc/%d/cmdline", pid );

    p = strchr ( cmdline, ' ' );
    if ( p )
	*p = '\0';

    p = strrchr ( cmdline, '/' );

    p = p ? p + 1 : cmdline;

    strncpy ( buf, p, buflen );
}

int si_main ( int argc, char **argv, char **envp )
{
    char myname[256];

    _get_myname ( myname, sizeof myname );
    printf ( "myname: %s\n", myname );

    exit (0);

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
// single instance
//

#include <sys/file.h>
#include <errno.h>

void
_single_instance ( char *lock_file )
{
    int fd;
    int rv;

    fd = open ( lock_file, O_CREAT | O_RDWR, 0666 );
    if ( fd < 0 )
    {
	perror ( "open" );
	exit ( 1 );
    }

    rv = flock ( fd, LOCK_EX | LOCK_NB );
    if ( rv < 0 )
    {
	if ( errno == EWOULDBLOCK )
	    printf ( "another instance is running\n" );
	else
	    perror ( "flock" );

	exit ( 2 );
    }

    //close ( fd );
}

int single_instatnce_main ( int argc, char **argv, char **envp )
{
    int fd;
    int rv;

    _single_instance ( "/tmp/whatever.pid" );
    
    // this is the first instance
    printf ( "first instance... sleep\n" );
    sleep ( 10 );
}

////////////////////////////////////////////////////////////////////////////////
//
//
//

#if 0
int
cpu_load ( int *min1, int *min5, int *min15 )
{
    char buf[] = "  1.4%% /   1.5%% /   1.7%%  ";
    float f1, f5, f15;
    int n;

    n = sscanf ( buf, "%f%%%% / %f%%%% / %f%%%%", &f1, &f5, &f15 );
    if ( n != 3 )
	return -1;

    printf ( "f1: %f, f5: %f, f15: %f\n", f1, f5, f15 );

    *min1  = round ( f1 );
    *min5  = round ( f5 );
    *min15 = round ( f15 );

    return 0;
}

int main ( int argc, char **argv, char **envp )
{
    int min1, min5, min15;

    cpu_load ( &min1, &min5, &min15 );

    printf ( "min1: %d, min5: %d, min15: %d\n", min1, min5, min15 );

    return 0;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//
//
//

#if 0
#define A_UNKNOWN	( 0 )
#define A_CPU		( 1 << 0 )
#define A_MEMORY	( 1 << 1 )
#define A_ETH0		( 1 << 2 )
#define A_WWAN		( 1 << 3 )
#endif

enum {
    A_UNKNOWN = -1,

    A_CPU,
    A_MEMORY,
    A_ETH0,
    A_WWAN,

    A_MAX
};
struct {
    int almid;
    char *almname;
} almtbl[] = {
    { A_CPU, "cpu" },
    { A_MEMORY, "memory" },
    { A_ETH0, "eth0" },
    { A_WWAN, "wwan" },
};

void alm_mask_str ( uint32_t mask, char *buf, int buf_len )
{
    int i, n;
    char *p = buf;

    *p = '\0';

    for ( i = 0; i < A_MAX; i++ )
    {
	if ( FLAG_ISSET(mask, 1 << i) )
	{
	    n = snprintf ( p, buf_len, " %s", almtbl[i].almname );
	    if ( n < 0 )
		break;

	    p += n;
	    buf_len -= n;

	    if ( buf_len < 0 )
		break;
	}
    }
}

int main_alm_mask ( int argc, char **argv, char **envp )
{
    char buf[10];

    alm_mask_str ( 1 << A_MEMORY |  1 << A_WWAN | 1 << A_ETH0,
	    buf, sizeof buf );

    printf("alarm str: %s\n", buf);

    return 0;
}

#endif

