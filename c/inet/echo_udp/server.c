#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "znet.h"

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

int
main ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int sd, rv, len;

    // socket address used to store client address
    socklen_t clnt_addr_len = 0;

    struct timeval tv;
    fd_set readfds, readfds_org;

    char buf[BUFSIZ];

#if 0
    sd = udp_socket ( &serv_addr, NULL, SERVER_PORT, ZNET_SERVER );
#else
    sd = _udp_socket_init ( &serv_addr, SERVER_PORT );
#endif
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

#if 0
		{
		int sd2;
		sd2 = socket(PF_INET, SOCK_DGRAM, 0);
		sendto(sd2, buf, len, 0, (struct sockaddr *)&clnt_addr,
			sizeof(clnt_addr));
		close(sd2);
		continue;
		}
#endif

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
main4 ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int sd, rv, len;

    // socket address used to store client address
    socklen_t clnt_addr_len = 0;

    struct timeval tv;
    fd_set readfds, readfds_org;

    char buf[BUFSIZ];

    // socket address used for the server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    serv_addr.sin_port = htons(SERVER_PORT);

    // htons: host to network long: same as htons but to long
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a UDP socket, creation returns -1 on failure
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(sd, (struct sockaddr *)&serv_addr,
		    sizeof(serv_addr))) < 0) {
	printf("could not bind socket\n");
	return 1;
    }

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

#if 0
		{
		int sd2;
		sd2 = socket(PF_INET, SOCK_DGRAM, 0);
		sendto(sd2, buf, len, 0, (struct sockaddr *)&clnt_addr,
			sizeof(clnt_addr));
		close(sd2);
		continue;
		}
#endif

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
main3 (int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int sd, rv, len;

    // socket address used to store client address
    socklen_t clnt_addr_len = 0;

    struct timeval tv;
    fd_set readfds, writefds;

    char buf[BUFSIZ];

    // socket address used for the server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    serv_addr.sin_port = htons(SERVER_PORT);

    // htons: host to network long: same as htons but to long
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a UDP socket, creation returns -1 on failure
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(sd, (struct sockaddr *)&serv_addr,
		    sizeof(serv_addr))) < 0) {
	printf("could not bind socket\n");
	return 1;
    }

    FD_ZERO ( &readfds );
    FD_ZERO ( &writefds );

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    // run indefinitely
    while (true) {

#if 1
	FD_SET ( sd, &readfds );

	rv = select ( sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv < 0 )
	{
	    printf("select error\n");
	    continue;
	}
	else if ( rv == 0 )
	{
	    tv.tv_sec = 10;
	    tv.tv_usec = 0;

	    putc ( '.', stdout );
	    fflush ( stdout );
	    continue;
	}

	printf("timeout remain: %ld sec %ld usec\n", tv.tv_sec, tv.tv_usec );
#endif

	clnt_addr_len = sizeof clnt_addr;

	// read content into buf from an incoming client
	len = recvfrom(sd, buf, sizeof(buf), 0,
		(struct sockaddr *)&clnt_addr,
		&clnt_addr_len);

	// inet_ntoa prints user friendly representation of the
	// ip address
	buf[len] = '\0';
	printf("received: '%s' from client %s\n", buf,
		inet_ntoa(clnt_addr.sin_addr));

	// send same content back to the client ("echo")
	sendto(sd, buf, len, 0, (struct sockaddr *)&clnt_addr,
		sizeof(clnt_addr));
    }

    return 0;
}

int main2(int argc, char *argv[]) {

    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons(SERVER_PORT);

    // htons: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a UDP socket, creation returns -1 on failure
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(sock, (struct sockaddr *)&server_address,
		    sizeof(server_address))) < 0) {
	printf("could not bind socket\n");
	return 1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    socklen_t client_address_len = 0;

    // run indefinitely
    while (true) {
	char buffer[500];

        client_address_len = sizeof client_address;

	// read content into buffer from an incoming client
	int len = recvfrom(sock, buffer, sizeof(buffer), 0,
		(struct sockaddr *)&client_address,
		&client_address_len);

	// inet_ntoa prints user friendly representation of the
	// ip address
	buffer[len] = '\0';
	printf("received: '%s' from client %s\n", buffer,
		inet_ntoa(client_address.sin_addr));

	// send same content back to the client ("echo")
	sendto(sock, buffer, len, 0, (struct sockaddr *)&client_address,
		sizeof(client_address));
    }

    return 0;
}
