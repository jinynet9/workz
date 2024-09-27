
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
    char *msg;

    if ( argc < 2 )
    {
	printf ( "a.out STRING\n" );
	exit (0);
    }

    msg = argv[1];

    sd = sock_uds_dgram_comm_init ( &my_addr, UDS_FILE_CLIENT,
	    &peer_addr, UDS_FILE_SERVER );
    if ( sd < 0 )
	return -1;

    peer_addr_len = sizeof peer_addr;

    printf ( "sending...\n" );
    rv = sendto ( sd, msg, strlen(msg) + 1, 0,
	    (struct sockaddr *)&peer_addr, peer_addr_len );
    if ( rv < 0 )
    {
	printf ( "sendto() rv:%d, err:%d,%s\n", rv,
		errno, strerror(errno) );
	return -1;
    }

    printf ( "receiving...\n" );
    rv = recvfrom ( sd, buf, sizeof buf, 0, NULL, NULL );
    if ( rv < 0 )
    {
	printf ( "recvfrom() rv:%d, err:%d,%s\n", rv,
		errno, strerror(errno) );
	return -1;
    }

    printf ( "receive: %s\n", buf );

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
    char *msg;

    if ( argc < 2 )
    {
	printf ( "a.out STRING\n" );
	exit (0);
    }

    msg = argv[1];

    sd = sock_uds_dgram_comm_init (
	    &peer_addr, UDS_FILE_SERVER,
	    &my_addr, UDS_FILE_CLIENT );
    if ( sd < 0 )
	return -1;

    peer_addr_len = sizeof peer_addr;

    sendto ( sd, msg, strlen(msg) + 1, 0,
	    (struct sockaddr *)&peer_addr, peer_addr_len );

    recvfrom ( sd, buf, sizeof buf, 0,
	    (struct sockaddr *)&peer_addr, &peer_addr_len );

    printf ( "receive: %s\n", buf );

    return 0;
}
#endif

