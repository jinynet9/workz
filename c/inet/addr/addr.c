#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define IP_ADDR_FORMAT		"%d.%d.%d.%d"
#define IP_ADDR_LIST(i)	\
    (unsigned char) (i)[0],	\
    (unsigned char) (i)[1],	\
    (unsigned char) (i)[2],	\
    (unsigned char) (i)[3]

#define MAC_ADDR_FORMAT		"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ADDR_LIST(m)	\
    (unsigned char) (m)[0],	\
    (unsigned char) (m)[1],	\
    (unsigned char) (m)[2],	\
    (unsigned char) (m)[3],	\
    (unsigned char) (m)[4],	\
    (unsigned char) (m)[5]

void
test_inet_pton ( int argc, char **argv )
{
    struct in_addr addr;
    char *p;
    char *ip = argv[1];
    char str[80];

    if ( argc < 2 )
    {
	printf ( "usage: %s IP-ADDRESS\n", argv[0] );
	return;
    }

    inet_pton ( AF_INET, ip, &addr.s_addr );

    p = (char *)&addr.s_addr;

    printf ( "%lu\n", addr.s_addr );
    printf ( IP_ADDR_FORMAT"\n", IP_ADDR_LIST(p) );

    inet_ntop ( AF_INET, &addr.s_addr, str, sizeof str );

    printf ( "%s\n", str );
}

void
test_prefix_len ( int argc, char **argv )
{
    int len, i, m;
    char *p;

    if ( argc < 2 )
    {
	printf ( "usage: %s PREFIX-LEN\n", argv[0] );
	return;
    }

    len = atoi ( argv[1] );

    p = (char *)&m;

    for ( i = 0; i < len; i++ )
	m |= 1 << 31 - i;

    m = htonl ( m );

    printf ( IP_ADDR_FORMAT"\n", IP_ADDR_LIST(p) );
}

int main ( int argc, char **argv, char **envp )
{

    //test_inet_pton ( argc, argv );
    test_prefix_len ( argc, argv );

    return 0;
}

