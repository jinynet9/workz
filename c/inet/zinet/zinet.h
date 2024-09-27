/*
 *
 */

#ifndef _ZINET_H_
#define _ZINET_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define SOCK_FLAG_SERVER	(1 << 0)
#define SOCK_FLAG_CLIENT	(1 << 1)
#define SOCK_FLAG_BIND		(1 << 2)

int
sock_dgram_init (
    struct sockaddr_in  *sa,
    char 		*domain,
    int 		port,
    uint32_t 		flag
    );

int
sock_uds_dgram_init (
    struct sockaddr_un  *sa,
    char 		*file,
    uint32_t 		flag
    );

int
sock_uds_dgram_comm_init (
    struct sockaddr_un  *sa,
    char 		*sfile,
    struct sockaddr_un  *ca,
    char 		*cfile
    );

int
sock_stream_init (
    struct sockaddr_in  *sa,
    char 		*domain,
    int 		port,
    uint32_t 		flag
    );

void
sock_flush_rxbuf ( int sd );

int
recv_to ( int sd, void *buf, int buflen, int flags, int to );

#ifdef __cplusplus
}
#endif

#endif /* _ZINET_H_ */
