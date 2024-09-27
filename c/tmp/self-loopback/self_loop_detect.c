/*
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "self_loop_detect.h"

static mac_t DST_MAC_ADDR;
static mac_t SRC_MAC_ADDR;
static mac_t SYSTEM_MAC_ADDR;

struct loopback
{
    uint16_t skip_count;
    uint16_t relevant_function;
    uint16_t receipt_number;

    uint32_t portno;
    unsigned long ttl_jiffies;
    char    tmp[32];
} __attribute__((__packed__));

#define LOOPBACK_TYPE	0x9000	/* loopback type */

static struct loopback _loopback;
static struct loopback    *_p_loopback = &_loopback;

#define MAX_PORT	4

struct sld_cfg {
    int interval;
    int btime;		/* block time */
} _cfg = {
    .interval = 60,
    .btime = 60,
};

struct sld_port
{
    int portno;		/* 0's base */
    int itimer;
    int detected;
    int bexpire;
};

static struct sld_port sld_ports[MAX_PORT];

/*
 *
 */
void
sld_init ( mac_t system_mac )
{
    int i;
    struct sld_port *port;

    memcpy ( SYSTEM_MAC_ADDR, system_mac, MAC_ADDR_LEN );

    /* make private mac address: 00:01:01 */
    SYSTEM_MAC_ADDR[0] = 0;
    SYSTEM_MAC_ADDR[1] = 1;
    SYSTEM_MAC_ADDR[2] = 1;

    memcpy ( SRC_MAC_ADDR, SYSTEM_MAC_ADDR, MAC_ADDR_LEN );
    memcpy ( DST_MAC_ADDR, SYSTEM_MAC_ADDR, MAC_ADDR_LEN );

    /* locally administered address */
    SRC_MAC_ADDR[0] |= 0x02;
    DST_MAC_ADDR[0] |= 0x06;

    /* trap to cpu */

    memset ( sld_ports, 0, sizeof sld_ports );

    for ( i = 0; i < MAX_PORT; i++ )
    {
	port = &sld_ports[i];

	port->portno = i;

	port->itimer = i * 3 + 1;
    }
}

/*
 *
 */
void
sld_config ( int interval, int btime )
{
    _cfg.interval = interval;
    _cfg.btime    = btime;
}

/*
 *
 */
static int
_is_loopframe ( struct eth_header *eth )
{
    /* frame type */
    if ( ENET_FRAME_TYPE(eth) != LOOPBACK_TYPE )
	return false;

    /* src mac address */
    if ( ! MAC_IS_EQUAL(eth->smac, SRC_MAC_ADDR) )
	return false;

    return true;
}

/*
 *
 */
int
sld_rx (
    int portno,
    void *_pkt
    )
{
    struct eth_header *eth;
    struct loopback *loopback;
    struct sld_port *port;

    if ( ! _is_loopframe(eth) )
	return -1;

    /* looped */

    port = &sld_ports[portno];

    if ( port->detected )
	return 0;

    loopback = ENET_PAYLOAD(eth);

    /* port block */

    SLD_DBG ( "port:%d", port->portno );

    port->detected = true;
    port->bexpire  = _cfg.btime;

    return 0;
}

/*
 *
 */
static void
_unblock_try ( struct sld_port *port )
{
    if ( --port->bexpire > 0 )
	return;

    /* port unblock */

    SLD_DBG ( "port:%d", port->portno );

    port->detected = false;
    port->itimer = 1;		/* for sending packet immediately */
}

/*
 *
 */
static void
_sld_tx ( struct sld_port *port )
{
    uint16_t vid = 0;

    if ( --port->itimer > 0 )
	return;

    _p_loopback->portno = port->portno;

    /*
    ether_frame_send (
    	port,
	DST_MAC_ADDR,
	SRC_MAC_ADDR,
	vid,
	LOOPBACK_TYPE,
	_p_loopback,
	sizeof(struct loopback)
	);
    */

    SLD_DBG ( "port:%d", port->portno );

    port->itimer = _cfg.interval;
}

/*
 *
 */
void
sld_1sec_polling ( void )
{
    int i;
    struct sld_port *port;

    for ( i = 0; i < MAX_PORT; i++ )
    {
	port = &sld_ports[i];

	if ( port->detected )
	    _unblock_try ( port );
	else
	{
#if 0
	    if ( link_down )
		continue;
#endif

	    _sld_tx ( port );
	}
    }
}

/*
 *
 */
void
sld_start ( void )
{
    /* start thread */

}

/*
 *
 */
void
sld_stop ( void )
{
    int i;
    struct sld_port *port;

    /* stop thread */

    for ( i = 0; i < MAX_PORT; i++ )
    {
	port = &sld_ports[i];

	if ( port->detected )
	    _unblock_try ( port );
    }
}

/*
 *
 */
int main ( void )
{
    return 0;
}

