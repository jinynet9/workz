
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "zinet.h"
#include "common.h"

/*
 *
 */
int
main ( int argc, char *argv[] )
{
    int rv, sd;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_len;

    char buf[BUFSIZ];

    sd = sock_uds_dgram_comm_init ( &my_addr, UDS_FILE_SERVER,
	    &peer_addr, UDS_FILE_CLIENT );
    if ( sd < 0 )
	return -1;

    while ( 1 )
    {

	printf ( "receiving...\n" );
	rv = recvfrom ( sd, buf, sizeof buf, 0, NULL, NULL );
	if ( rv < 0 )
	{
	    printf ( "recvfrom() rv:%d, err:%d,%s\n", rv,
		    errno, strerror(errno) );
	    continue;
	}

	printf ( "receive: %s\n", buf );

	peer_addr_len = sizeof peer_addr;

	rv = sendto ( sd, buf, strlen(buf) + 1, 0,
		(struct sockaddr *)&peer_addr, peer_addr_len );
	if ( rv < 0 )
	{
	    printf ( "sendto() rv:%d, err:%d,%s\n", rv,
		    errno, strerror(errno) );

	}
    }

    return 0;
}

#if 0
/*
 *
 */
int
main ( int argc, char *argv[] )
{
    int sd;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_len;

    char buf[BUFSIZ];

    sd = sock_uds_dgram_init ( &my_addr, UDS_FILE_SERVER, SOCK_FLAG_SERVER );
    if ( sd < 0 )
	return -1;

    while ( 1 )
    {
	peer_addr_len = sizeof peer_addr;

	recvfrom ( sd, buf, sizeof buf, 0,
		(struct sockaddr *)&peer_addr, &peer_addr_len );

	printf ( "receive: %s\n", buf );

	sendto ( sd, buf, strlen(buf) + 1, 0,
		(struct sockaddr *)&peer_addr, peer_addr_len );
    }

    return 0;
}
#endif

