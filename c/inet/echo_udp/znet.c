/*
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "znet.h"

void
sockaddr_in_init (
    struct sockaddr_in  *sa,
    char		*domain,
    int			port
    )
{
    memset ( sa, 0, sizeof(*sa) );

    sa->sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    if ( domain )
    inet_pton ( AF_INET, domain, &sa->sin_addr );

    // htons: port in network order format
    sa->sin_port = htons(port);
}

int
udp_socket (
    struct sockaddr_in  *sa,
    char		*domain,
    int			port,
    int			type
    )
{
    int sd, rv;

    // open socket
    sd = socket ( PF_INET, SOCK_DGRAM, 0 );
    if ( sd < 0 )
    {
	printf("could not create socket\n");
	return -1;
    }

    sockaddr_in_init ( sa, domain, port );

    if ( type == ZNET_SERVER )
    {
	rv = bind ( sd, (struct sockaddr *)sa, sizeof(*sa) );
	if ( rv < 0 )
	{
	    printf("could not bind socket\n");
	    return -2;
	}
    }
#if 0
    else
    {
	rv = connect ( sd, (struct sockaddr *)sa, sizeof(*sa) );
	if ( rv < 0 )
	{
	    printf("connection error\n");
	    return -3;
	}
    }
#endif

    return sd;
}
