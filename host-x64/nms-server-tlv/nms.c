/*
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "zut.h"
#include "zlogger.h"

#include "log.h"
#include "message.h"

/* dummy */
char sys_base_mac[6];

// port to start the server on
#define SERVER_PORT	9090

static int
_udp_socket_init ( struct sockaddr_in *sa, int port )
{
    int sd, rv;

    sd = socket ( PF_INET, SOCK_DGRAM, 0 );
    if ( sd < 0 )
    {
	printf("could not create socket\n");
	return -1;
    }

    memset ( sa, 0, sizeof(*sa) );

    sa->sin_family = AF_INET;
    sa->sin_port = htons ( port );

    rv = bind ( sd, (struct sockaddr *)sa, sizeof(*sa) );
    if ( rv < 0 )
    {
	printf("could not bind socket\n");
	return -2;
    }

    return sd;
}

/*
 *
 */
int
main ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int sd, rv, len;

    // socket address used to store client address
    socklen_t clnt_addr_len;

    char buf[BUFSIZ];

    zlog_level_set ( ZLOG_DEBUG );

    sd = _udp_socket_init ( &serv_addr, SERVER_PORT );
    if ( sd < 0 )
	return -1;

    // run indefinitely
    while (true)
    {
	clnt_addr_len = sizeof clnt_addr;

	// read content into buf from an incoming client
	len = recvfrom(sd, buf, sizeof(buf), 0,
		(struct sockaddr *)&clnt_addr,
		&clnt_addr_len);
	if ( len < 0 )
	{
	    printf("recvfrom error !!!\n");
	    continue;
	}

	decode_msg ( buf, len );
    }

    close ( sd );

    return 0;
}

/*
 * echo server
 */
int
main_ ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int sd, rv, len;

    // socket address used to store client address
    socklen_t clnt_addr_len;

    struct timeval tv;
    fd_set readfds, readfds_org;

    char buf[BUFSIZ];

    zlog_level_set ( ZLOG_DEBUG );

    sd = _udp_socket_init ( &serv_addr, SERVER_PORT );
    if ( sd < 0 )
	return -1;

    FD_ZERO ( &readfds_org );

    FD_SET ( sd, &readfds_org );

    // run indefinitely
    while (true) {

	readfds = readfds_org;

	tv.tv_sec = 10;
	tv.tv_usec = 0;

	rv = select ( sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv < 0 )
	{
	    printf("select error\n");
	    continue;
	}
	else if ( rv == 0 )	/* timeout */
	{
	    putc ( '.', stdout );
	    fflush ( stdout );
	    continue;
	}
	else
	{
	    printf("timeout remain: %ld sec %ld usec\n", tv.tv_sec, tv.tv_usec );

	    if ( FD_ISSET(sd, &readfds) )
	    {
		clnt_addr_len = sizeof clnt_addr;

		// read content into buf from an incoming client
		len = recvfrom(sd, buf, sizeof(buf), 0,
			(struct sockaddr *)&clnt_addr,
			&clnt_addr_len);
		if ( len < 0 )
		{
		    printf("recvfrom error !!!\n");
		    continue;
		}

		// inet_ntoa prints user friendly representation of the
		// ip address
		buf[len] = '\0';
		printf("received: '%s' from client %s\n", buf,
			inet_ntoa(clnt_addr.sin_addr));

		// send same content back to the client ("echo")
		sendto(sd, buf, len, 0, (struct sockaddr *)&clnt_addr,
			sizeof(clnt_addr));
	    }
	}
    }

    close ( sd );

    return 0;
}

int
main3 ( int argc, char **argv, char **envp )
{
    zlog_level_set ( ZLOG_DEBUG );

    NMS_LOG ( "hello nms..." );

    return 0;
}

