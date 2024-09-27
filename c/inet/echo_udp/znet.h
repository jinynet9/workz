/*
 *
 */

#ifndef _ZNET_H_
#define _ZNET_H_

void
sockaddr_in_init (
    struct sockaddr_in  *saddr,
    char		*domain,
    int			port
    );

#define ZNET_SERVER	0
#define ZNET_CLIENT	1

int
udp_socket (
    struct sockaddr_in  *sa,
    char		*domain,
    int			port,
    int			type
    );

#endif	/* _ZNET_H_ */

