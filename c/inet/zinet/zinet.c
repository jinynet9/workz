/*
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "zut.h"

#include "zinet.h"

static int
_bind ( int sd, void *sa, int sa_len )
{
    int rv, val = 1;

    rv = setsockopt ( sd, SOL_SOCKET, SO_REUSEADDR,
	    (char *) &val, sizeof val );
    if ( rv < 0 )
	return -1;

    rv = bind ( sd, sa, sa_len );
    if ( rv < 0 )
	return -1;

    return 0;
}

/*
 *
 */
int
sock_dgram_init (
    struct sockaddr_in  *sa,
    char 		*domain,
    int 		port,
    uint32_t 		flag
    )
{
    int sd, rv;

    sd = socket ( PF_INET, SOCK_DGRAM, 0 );
    if ( sd < 0 )
	return -1;

    memset ( sa, 0, sizeof(*sa) );

    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = htonl ( INADDR_ANY );
    sa->sin_port = htons ( port );

    if ( FLAG_ISSET(flag, SOCK_FLAG_CLIENT) )
    {
	inet_pton ( AF_INET, domain, &sa->sin_addr );
    }

    if ( FLAG_ISSET(flag, SOCK_FLAG_SERVER) ||
         FLAG_ISSET(flag, SOCK_FLAG_BIND) )
    {
	rv = _bind ( sd, (struct sockaddr *)sa, sizeof(*sa) );
	if ( rv < 0 )
	    return -1;
    }

    return sd;
}

/*
 *
 */
int
sock_uds_dgram_init (
    struct sockaddr_un  *sa,
    char 		*file,
    uint32_t 		flag
    )
{
    int sd, rv;

    if ( FLAG_ISSET(flag, SOCK_FLAG_SERVER) ||
         FLAG_ISSET(flag, SOCK_FLAG_BIND) )
    {
	rv = access ( file, F_OK );
	if ( rv == 0 )
	    unlink ( file );
    }

    /* open UDS socket */
    sd = socket ( AF_UNIX, SOCK_DGRAM, 0 );
    if ( sd < 0 )
	return -1;

    memset ( sa, 0, sizeof(*sa) );

    sa->sun_family = AF_UNIX;
    strcpy ( sa->sun_path, file );

    if ( FLAG_ISSET(flag, SOCK_FLAG_SERVER) ||
         FLAG_ISSET(flag, SOCK_FLAG_BIND) )
    {
	rv = _bind ( sd, (struct sockaddr *)sa, sizeof(*sa) );
	if ( rv < 0 )
	    return -1;
    }

    return sd;
}

/*
 *
 */
int
sock_uds_dgram_comm_init (
    struct sockaddr_un  *sa,		// my, binding
    char 		*sfile,
    struct sockaddr_un  *ca,		// peer
    char 		*cfile
    )
{
    int sd;

    sd = sock_uds_dgram_init ( sa, sfile, SOCK_FLAG_BIND );
    if ( sd < 0 )
	return -1;

    memset ( ca, 0, sizeof(*ca) );

    ca->sun_family = AF_UNIX;
    strcpy ( ca->sun_path, cfile );

    return sd;
}

/*
 *
 */
int
sock_stream_init (
    struct sockaddr_in  *sa,
    char 		*domain,
    int 		port,
    uint32_t 		flag
    )
{
    int sd, rv;

    sd = socket ( PF_INET, SOCK_STREAM, 0 );
    if ( sd < 0 )
	return -1;

    memset ( sa, 0, sizeof(*sa) );

    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = htonl ( INADDR_ANY );
    sa->sin_port = htons ( port );

    if ( FLAG_ISSET(flag, SOCK_FLAG_CLIENT) )
    {
	inet_pton ( AF_INET, domain, &sa->sin_addr );
    }

    if ( FLAG_ISSET(flag, SOCK_FLAG_SERVER) ||
         FLAG_ISSET(flag, SOCK_FLAG_BIND) )
    {
	rv = _bind ( sd, (struct sockaddr *)sa, sizeof(*sa) );
	if ( rv < 0 )
	    return -1;
    }

    return sd;
}

/*
 *
 */
void
sock_flush_rxbuf ( int sd )
{
    fd_set readfds, readfds_org;
    char buf[BUFSIZ];
    struct timeval tv;
    int rv;

    FD_ZERO ( &readfds_org );

    FD_SET ( sd, &readfds_org );

    memset ( &tv, 0, sizeof tv );

    while ( true )
    {
	readfds = readfds_org;

	rv = select ( sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv > 0 && FD_ISSET(sd, &readfds) )
	    recv ( sd, buf, sizeof buf, 0 );
	else
	    break;
    }
}

static int
_sock_read_to ( int sd, int to )
{
    fd_set readfds;
    int max_sd, rv;
    struct timeval tv;

    FD_ZERO ( &readfds );

    FD_SET ( sd, &readfds );

    max_sd = sd;

    memset ( &tv, 0, sizeof tv );
    tv.tv_sec = to;

    while ( true )
    {
	rv = select ( max_sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv < 0 )
	{
	    if ( errno == EINTR || errno != EAGAIN )
		continue;

	    return -1;
	}
	else if ( rv == 0 )	/* timeout */
	    return 0;
	else
	{
	    if ( FD_ISSET(sd, &readfds) )
		return 1;

	    /* internal error */
	    return -1;
	}
    }
}

/*
 *
 */
int
recv_to ( int sd, void *buf, int buflen, int flags, int to )
{
    int rv;

    rv = _sock_read_to ( sd, to );
    if ( rv <= 0 )
	return rv;

    return recv ( sd, buf, buflen, flags );
}

