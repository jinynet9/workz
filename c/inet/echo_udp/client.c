#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>

#include "znet.h"

#define SERVER_DOMAIN	"localhost"
#define SERVER_PORT	9090

static int
_udp_socket_init ( struct sockaddr_in *sa, char *domain, int port )
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

    inet_pton ( AF_INET, domain, &sa->sin_addr );


    return sd;
}

int
main ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    int sd;
    int rv, len;
    char msg[BUFSIZ];
    char buf[BUFSIZ];

    struct timeval tv;
    fd_set readfds, readfds_org;

    // open socket
#if 0
    sd = udp_socket ( &serv_addr, SERVER_DOMAIN, SERVER_PORT, ZNET_CLIENT );
#else
    sd = _udp_socket_init ( &serv_addr, SERVER_DOMAIN, SERVER_PORT );
#endif
    if ( sd < 0 )
	return 1;

    rv = connect ( sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
    if ( rv < 0 )
    {
	printf("connection error\n");
	return 1;
    }

    FD_ZERO ( &readfds_org );

    FD_SET ( sd, &readfds_org );

    while ( true )
    {
	fputs("Input message(Q to quit): ", stdout);
	fflush(stdout);
	fgets(msg, sizeof msg, stdin);

	len = strlen(msg);
	if ( msg[len-1] == '\n' )
	{
	    len--;
	    msg[len] = '\0';
	}

	if(!strcmp(msg,"q") || !strcmp(msg,"Q"))
	    break;

	// send data
	len = write(sd, msg, strlen(msg));
	printf("sent: len(%d)\n", len);

	tv.tv_sec = 2;
	tv.tv_usec = 0;

retry:
	readfds = readfds_org;

	rv = select ( sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv < 0 )
	{
	    printf("select error !!\n");
	    continue;
	}
	else if ( rv == 0 )
	{
	    printf("timeout !!\n");
	    continue;
	}
	else
	{
	    if ( FD_ISSET(sd, &readfds) )
	    {
		// received echoed data back
#if 0
		len = read(sd, buf, sizeof buf);
#else
		struct sockaddr_in peer_addr;
		socklen_t peer_addr_len;

		peer_addr_len = sizeof peer_addr;

		// read content into buf from an incoming client
		len = recvfrom(sd, buf, sizeof buf, 0,
			(struct sockaddr *)&peer_addr, &peer_addr_len);
#endif
		if ( len < 0 )
		{
		    printf("read error, len(%d)\n", len);
		    printf("errno[%d]: %s\n", errno, strerror(errno));
		    goto retry;
		}
		else if ( len == 0 )
		{
		    printf("the peer has performed an orderly shutdown\n");
		    goto retry;
		}

		buf[len] = '\0';
		//printf("rv(%d), len(%d), recieved: '%s'\n", rv, len, buf);
		printf("rv(%d), len(%d), recieved: '%s' from %s:%d\n",
			rv, len, buf,
			inet_ntoa(peer_addr.sin_addr),
			ntohs(peer_addr.sin_port));
	    }
	    else
		printf("rv(%d)\n", rv);
	}
    }

    // close the socket
    close(sd);
    return 0;
}

int
main4 ( int argc, char *argv[] )
{
    struct sockaddr_in serv_addr;
    int sd;
    int rv, len;
    char msg[BUFSIZ];
    char buf[BUFSIZ];

    struct timeval tv;
    fd_set readfds, readfds_org;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, SERVER_DOMAIN, &serv_addr.sin_addr);

    // htons: port in network order format
    serv_addr.sin_port = htons(SERVER_PORT);

    // open socket
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    rv = connect ( sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
    if ( rv < 0 )
    {
	printf("connection error\n");
	return 1;
    }

    FD_ZERO ( &readfds_org );

    FD_SET ( sd, &readfds_org );

    while ( true )
    {
	fputs("Input message(Q to quit): ", stdout);
	fflush(stdout);
	fgets(msg, sizeof msg, stdin);

	len = strlen(msg);
	if ( msg[len-1] == '\n' )
	{
	    len--;
	    msg[len] = '\0';
	}

	if(!strcmp(msg,"q") || !strcmp(msg,"Q"))
	    break;

	// send data
	len = write(sd, msg, strlen(msg));
	printf("sent: len(%d)\n", len);

	tv.tv_sec = 2;
	tv.tv_usec = 0;

retry:
	readfds = readfds_org;

	rv = select ( sd + 1, &readfds, NULL, NULL, &tv );
	if ( rv < 0 )
	{
	    printf("select error !!\n");
	    continue;
	}
	else if ( rv == 0 )
	{
	    printf("timeout !!\n");
	    continue;
	}
	else
	{
	    if ( FD_ISSET(sd, &readfds) )
	    {
		// received echoed data back
#if 0
		len = read(sd, buf, sizeof buf);
#else
		struct sockaddr_in peer_addr;
		socklen_t peer_addr_len;

		peer_addr_len = sizeof peer_addr;

		// read content into buf from an incoming client
		len = recvfrom(sd, buf, sizeof buf, 0,
			(struct sockaddr *)&peer_addr, &peer_addr_len);
#endif
		if ( len < 0 )
		{
		    printf("read error, len(%d)\n", len);
		    goto retry;
		}

		buf[len] = '\0';
		//printf("rv(%d), len(%d), recieved: '%s'\n", rv, len, buf);
		printf("rv(%d), len(%d), recieved: '%s' from %s:%d\n",
			rv, len, buf,
			inet_ntoa(peer_addr.sin_addr),
			ntohs(peer_addr.sin_port));
	    }
	    else
		printf("rv(%d)\n", rv);
	}
    }

    // close the socket
    close(sd);
    return 0;
}

int main3() {
    int sd;
    int rv, len;
    char msg[BUFSIZ];
    char buf[BUFSIZ];

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, SERVER_DOMAIN, &serv_addr.sin_addr);

    // htons: port in network order format
    serv_addr.sin_port = htons(SERVER_PORT);

    // open socket
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    rv = connect ( sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
    if ( rv < 0 )
    {
	printf("connection error\n");
	return 1;
    }

    while ( 1 )
    {
	fputs("Input message(Q to quit): ", stdout);
	fgets(msg, sizeof msg, stdin);

	len = strlen(msg);
	if ( msg[len-1] == '\n' )
	{
	    len--;
	    msg[len] = '\0';
	}

	if(!strcmp(msg,"q") || !strcmp(msg,"Q"))
	    break;

	// send data
	len = send(sd, msg, strlen(msg), 0);

	// received echoed data back
	len = recv(sd, buf, sizeof buf, 0);

	buf[len] = '\0';
	printf("recieved: '%s'\n", buf);
    }

    // close the socket
    close(sd);
    return 0;
}

int main2 () {
    int sd;
    int len;
    char msg[BUFSIZ];
    char buf[BUFSIZ];

    struct sockaddr_in serv_addr, peer_addr;
    socklen_t peer_addr_len;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, SERVER_DOMAIN, &serv_addr.sin_addr);

    // htons: port in network order format
    serv_addr.sin_port = htons(SERVER_PORT);

    // open socket
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
    }

    while ( 1 )
    {
	fputs("Input message(Q to quit): ", stdout);
	fgets(msg, sizeof msg, stdin);

	len = strlen(msg);
	if ( msg[len-1] == '\n' )
	{
	    len--;
	    msg[len] = '\0';
	}

	if(!strcmp(msg,"q") || !strcmp(msg,"Q"))
	    break;

	// send data
	len =
	    sendto(sd, msg, strlen(msg), 0,
		    (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	// received echoed data back
#if 0
	len = recvfrom(sd, buf, sizeof buf, 0, NULL, NULL);

	buf[len] = '\0';
	printf("recieved: '%s'\n", buf);
#else
	peer_addr_len = sizeof peer_addr;
	len = recvfrom(sd, buf, sizeof buf, 0,
			(struct sockaddr *)&peer_addr, &peer_addr_len);

	buf[len] = '\0';
	printf("recieved: '%s' from %s:%d\n",
		buf,
		inet_ntoa(peer_addr.sin_addr),
		ntohs(peer_addr.sin_port));
#endif
    }

    // close the socket
    close(sd);
    return 0;
}
