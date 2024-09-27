/*
 *
 */

#ifndef _ZUT_H_
#define _ZUT_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include <sys/wait.h>	/* WEXITSTATUS */


/*
 *
 */
#define MEMCLR(p, n)		memset(p, 0, n)
#define DIM(v)			( sizeof(v)/sizeof(v[0]) )
#define UNUSED_WARNFREE(X)	(void)X


#define FLAG_SET(v,n)          do { (v) |= (n); } while (0)
#define FLAG_CLR(v,n)          do { (v) &= ~(n); } while (0)
#define FLAG_ISSET(v,n)        ( (v) & (n) ? 1 : 0 )

#define ATTR_PACKED	__attribute__ ((packed))

#ifndef MAX
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define IF_ERRET(op)	\
    do { int __rv__; if ((__rv__ = (op)) < 0) return(__rv__); } while(0) 


/*
 *
 */
#define streq(A, B)							\
    ({									\
        const void *_x = A, *_y = B; int _v;				\
        _v = _x && _y && 0 == strcmp(_x, _y);				\
        _v;								\
     })

#define streqn(A, B)							\
    ({									\
        const void *_x = A, *_y = B; int _v;				\
        _v = _x && _y && 0 == strncmp(_x, _y, strlen(_y));		\
        _v;								\
     })

#define strneq(A, B, N)							\
    ({									\
        const void *_x = A, *_y = B; int _v;				\
        _v = _x && _y && 0 == strncmp(_x, _y, N);			\
        _v;								\
     })

/* Like strncpy but make sure the resulting string is always 0 terminated. */
#define Strcpy(D,S)                                             \
    do {                                                        \
        char *_d = D, *_s = S;                                  \
        if ( _d ) _d[0] = '\0';                                 \
        if ( _d && _s )   strcpy(_d,_s);                        \
    } while (0)
#define Strncpy(D,S,L)                                          \
    do {                                                        \
        char *_d = D, *_s = S;                                  \
        if ( _d ) (_d)[0] = '\0';                               \
        if ( _d && _s ) (_d)[(L)-1]='\0', strncpy(_d,_s,(L)-1); \
    } while (0)
#define Strncat(D,S,L)                  			\
    do {                                			\
        char *_d = D, *_s = S;          			\
        if ( _d && _s )                 			\
        {                               			\
            strncat(_d,_s,(L)-1);       			\
            (_d)[(L)-1] = '\0';         			\
        }                               			\
    } while (0)


/*
 *
 */
int Rand ( void );

#define RAND_RANGE(MIN, MAX)						\
    ( (Rand() % ((MAX) - (MIN) + 1)) + (MIN) )


/*
 *
 */
typedef   signed char       s8;
typedef unsigned char       u8;
typedef   signed short      s16;
typedef unsigned short      u16;
typedef   signed int        s32;
typedef unsigned int        u32;
typedef   signed long long  s64;
typedef unsigned long long  u64;
//typedef enum { false, true } bool;	/* stdbool.h */


/*
 *
 */
int
shell_read (
    char 		*b,
    int 		blen,
    uint32_t		flags,
    const char 		*fmt, ... );

#define SF_TRIM_NEWLINE		(1 << 0)

/* trim new line */
#define shell_read_tnl(b, blen, fmt, arg...)		\
    shell_read (b, blen, SF_TRIM_NEWLINE, fmt, ##arg )


#define SHELL_READ(V, fmt, arg...)			\
    shell_read_tnl ( V, sizeof(V), fmt, ##arg )

#define CAT_READ(V, fmt, arg...)			\
    shell_read_tnl ( V, sizeof(V), "cat "fmt, ##arg )


#define SYSLOG(LEVEL, fmt, arg...)					\
    do {								\
	syslog ( LOG_LOCAL7|LEVEL, fmt, ##arg );			\
    } while (0)

#define SYSTEM(fmt, arg...)						\
    ({									\
	char _buf[4096];						\
	int _rv, _ret = -1;						\
	snprintf ( _buf, sizeof _buf, fmt, ##arg );			\
	_rv = system ( _buf );						\
	if ( _rv == 127 ) {						\
	    /* can't execute /bin/sh */					\
     	    /* a shell could not be executed in the child process */	\
	} else if ( _rv == -1 ) {					\
	    /* fork error */						\
	    /* errno is set to indicate the error */			\
	} else {							\
	    _ret = WEXITSTATUS(_rv);					\
	}								\
	_ret;								\
    })

#define MAC_ADDR_FORMAT		"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ADDR_LIST(m)	\
    (unsigned char) (m)[0],	\
    (unsigned char) (m)[1],	\
    (unsigned char) (m)[2],	\
    (unsigned char) (m)[3],	\
    (unsigned char) (m)[4],	\
    (unsigned char) (m)[5]


void skeleton_daemon ( void );

void single_instance ( char *lock_file );

////////////////////////////////////////////////////////////////////////////////
// network {
//

int sys_mem_usage ( void );
int sys_cpu_usage ( int *, int *, int * );
uint32_t sys_uptime ( void );

#include <netinet/in.h>
#include <net/if.h>

struct net_stats_s
{
    char bytes[16];
    char packets[16];
    char errors[16];

    uint64_t nbytes;
    uint64_t npackets;
    uint64_t nerrors;
};

struct net_if_info_s {
    int			link;
    struct in_addr 	addr;
    struct in_addr 	mask;
    char                mac[IFHWADDRLEN];

    struct net_stats_s	rx_stats;
    struct net_stats_s	tx_stats;
};

#define IFINFO_LINK	(1 << 0)
#define IFINFO_STATS	(1 << 1)
#define IFINFO_SOCK	(1 << 2)
#define IFINFO_IPADDR	(1 << 3)
#define IFINFO_IPMASK	(1 << 4)
#define IFINFO_MACADDR	(1 << 5)

int net_if_info_get ( char *, struct net_if_info_s *, uint32_t );

//
// network }

////////////////////////////////////////////////////////////////////////////////
// UCI {
//

#define UCI			"uci -q "

#define UCI_GET(N, V)							\
    do {								\
	V[0] = '\0';							\
	shell_read_tnl ( V, sizeof(V), UCI" get %s", N ); 		\
    } while (0)

#define UCI_SET(N, V)							\
    do {								\
	SYSTEM ( UCI" set %s=\'%s\'", N, V );				\
    } while (0)

//
// UCI }

#ifdef __cplusplus
}
#endif

#endif	/* _ZUT_H_ */
