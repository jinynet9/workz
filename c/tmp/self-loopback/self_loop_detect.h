
#ifndef SELF_LOOP_DETECT_H
#define SELF_LOOP_DETECT_H

/*
 * http://lwn.net/Articles/330797/
 * For ECTP numeric fields, the network order is little endian,
 * the opposite to the network order of IPv4
 */
#if 1 /* __BIG_ENDIAN */
# define l_htons(A)	( (((uint16_t)(A) & 0xff00) >> 8) | \
			  (((uint16_t)(A) & 0x00ff) << 8) )
# define l_htonl(A)	( (((uint32_t)(A) & 0xff000000) >> 24) | \
			  (((uint32_t)(A) & 0x00ff0000) >> 8) | \
			  (((uint32_t)(A) & 0x0000ff00) << 8) | \
			  (((uint32_t)(A) & 0x000000ff) << 24) )
# define l_ntohs	l_htons
# define l_ntohl	l_htonl
#else /* __LITTLE_ENDIAN */
# define l_htons(A)	(A)
# define l_htonl(A)	(A)
# define l_ntohs(A)	(A)
# define l_ntohl(A)	(A)
#endif

#define MAC_ADDR_LEN	(6)

typedef unsigned char mac_t[MAC_ADDR_LEN];
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

#define MAC_IS_EQUAL(m1, m2)    					\
    	({ void *_x = m1, *_y = m2; int _v;				\
	   _v = _x && _y && 0 == memcmp(_x, _y, MAC_ADDR_LEN);		\
	   _v;								\
	})

#define true	1
#define false	0

#define SLD_DBG(fmt, arg...)				\
  do {							\
    printf ( "%s():: "fmt"\n", __func__, ##arg );	\
  } while (0)

/*
 * ethernet header
 */

#define ENET_UNTAGGED_HDR_LEN   14
#define ENET_TAGGED_HDR_LEN     18
#define ENET_TAG_SIZE           4

/* VLAN header */
struct vlan_header
{
    uint16_t tag_type;      /* 0x8100 */
    uint16_t pri_cif_vid;   /* 3 bits priority
			       1 bit canonical format indicator
			       12 bits VLAN identifier. */
    uint16_t type;          /* Ethernet frame type. */
} __attribute__((__packed__));

/* Ethernet header */
struct eth_header
{
    uint8_t dmac[6];
    uint8_t smac[6];
    union
    {
	uint16_t type;
	struct vlan_header vlan;
    } u;
} __attribute__((__packed__));


#define ENET_8021Q_VLAN                0x8100

#define ENET_TAGGED(e) \
    ( ntohs((e)->u.type) == ENET_8021Q_VLAN )

#define ENET_FRAME_TYPE(e)   		\
    ntohs ( ENET_TAGGED(e) ? (e)->u.vlan.type : (e)->u.type )

#define ENET_PAYLOAD(e)                 \
  ({                                    \
    char *EP = (char *)e;		\
    EP += ENET_TAGGED(e) ? ENET_TAGGED_HDR_LEN : ENET_UNTAGGED_HDR_LEN;  \
    (void *)EP;				\
  })

#endif	/* SELF_LOOP_DETECT_H */

