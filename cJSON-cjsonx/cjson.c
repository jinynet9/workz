/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <netinet/in.h>

#include "cJSON.h"

#include "zinet.h"
#include "cjson_log.h"

#define Strncpy(D,S,L)                                          \
    do {                                                        \
        char *_d = D, *_s = S;                                  \
        if ( _d ) (_d)[0] = '\0';                               \
        if ( _d && _s ) (_d)[(L)-1]='\0', strncpy(_d,_s,(L)-1); \
    } while (0)


cJSON *_root;
cJSON *_curr;
cJSON *_arrl;	/* array element */

struct sockaddr_un g_cjson_addr;

int json_sd;

#define JSON_TMP_PATH	"."

#define UDS_FILE_SERVER		JSON_TMP_PATH"/cjson_server.uds"
#define UDS_FILE_CLIENT		JSON_TMP_PATH"/cjson_client.uds"

/*
 * _printbuf[0] : return state value
 */
#define PRINT_BUFLEN		( 64 * 1024 )
static char _printbuf[PRINT_BUFLEN + 2];
#define PRINTBUF		( &_printbuf[1] )

struct json_event {
    char cmd[80];
    char name[80];
    char value[4096];
};

static int
json_init ( char *name, char *value )
{
    cJSON_Destroy ( _root );

    _root = _curr = cJSON_CreateObject ();
    if ( ! _root )
	return -1;

    _arrl = NULL;

    return 0;
}

static int
_json_dump ( cJSON *node, cJSON_bool format )
{
#if 0
    char *out;

    out = cJSON_Print ( node );
    if ( ! out )
	return -1;

    snprintf ( PRINTBUF, PRINT_BUFLEN, "%s", out );

    cJSON_PrintFree ( out );
#else
    int rv;

    rv = cJSON_PrintPreallocated ( node, PRINTBUF, PRINT_BUFLEN, format );
    if ( rv != true )
	return -1;
#endif

    return 0;
}

static int
json_dump ( char *name, char *value )
{
    cJSON_bool format;

    format = ( value[0] == '1' ) ? true : false;

    return _json_dump ( _curr, format );
}

static int
json_dump_root ( char *name, char *value )
{
    cJSON_bool format;

    format = ( value[0] == '1' ) ? true : false;

    return _json_dump ( _root, format );
}

static int
json_add_int ( char *name, char *value )
{
    int int_val = atoi ( value );
    cJSON *node;

    if ( _curr->type == cJSON_Array )
	node = cJSON_AddIntToArray ( _curr, int_val );
    else
	node = cJSON_AddIntToObject ( _curr, name, int_val );

    if ( ! node )
	return -1;

    return 0;
}

static int
json_add_string ( char *name, char *value )
{
    cJSON *node;

    if ( _curr->type == cJSON_Array )
	node = cJSON_AddStringToArray ( _curr, value );
    else
	node = cJSON_AddStringToObject ( _curr, name, value );

    if ( ! node )
	return -1;

    return 0;
}

static int
json_add_bool ( char *name, char *value )
{
    int int_val = atoi ( value );
    cJSON *node;

    if ( _curr->type == cJSON_Array )
	node = cJSON_AddBoolToArray ( _curr, int_val );
    else
	node = cJSON_AddBoolToObject ( _curr, name, int_val );

    if ( ! node )
	return -1;

    return 0;
}

static int
json_add_object ( char *name, char *value )
{
    int rv;

    cJSON *node = cJSON_CreateObject ();
    if ( ! node )
	return -1;

    rv = cJSON_AddItemToObject ( _curr, name, node );
    if ( rv != true )
    {
	cJSON_Delete ( node );
	return -1;
    }

    _curr = node;

    return 0;
}

static int
json_close_object ( char *name, char *value )
{
    if ( _curr->parent )
	_curr = _curr->parent;

    return 0;
}

static int
json_add_array ( char *name, char *value )
{
    int rv;

    cJSON *node = cJSON_CreateArray ();
    if ( ! node )
	return -1;

    rv = cJSON_AddItemToObject ( _curr, name, node );
    if ( rv != true )
    {
	cJSON_Delete ( node );
	return -1;
    }

    _curr = node;

    return 0;
}

static int
json_close_array ( char *name, char *value )
{
    if ( _curr->parent )
	_curr = _curr->parent;

    return 0;
}

static int
json_load_file ( char *name, char *value )
{
    cJSON *node;

    node = cJSON_LoadFile ( name );
    if ( ! node )
	return -1;

    cJSON_Destroy ( _root );

    _root = _curr = node;

    return 0;
}

static int
_node_print ( cJSON *node )
{
    char buf[256], *p = buf;
    int rv, l;
    cJSON_bool format = true;

    if ( node->type == cJSON_True || node->type == cJSON_False )
    {
	sprintf ( p, "%d", node->type == cJSON_True ? 1 : 0 );
    }
    else
    {
	rv = cJSON_PrintPreallocated ( node, buf, sizeof(buf), format );
	if ( rv != true )
	    return -1;

	/* remove "" from value */
#if 0
	l = strlen ( buf );
	if ( buf[0] == '"' && buf[l-1] == '"' )
	{
	    p = buf + 1;
	    buf[l-1] = '\0';
	}
#else
	if ( node->type == cJSON_String )
	{
	    p = buf + 1;

	    l = strlen ( buf );
	    buf[l-1] = '\0';
	}
#endif
    }

    strcpy ( PRINTBUF, p );

    return 0;
}

static int
json_get_var ( char *name, char *value )
{
    cJSON *node;
    int rv;

    node = cJSON_Lookup ( _curr->child, name );
    if ( ! node )
	return -1;

    rv = _node_print ( node );
    if ( rv < 0 )
	return -1;

    return 0;
}

static int
json_get_var_array ( char *name, char *value )
{
    int rv;

    if ( strcmp(name, "1") == 0 && (_curr->type & 0xFF) == cJSON_Array )
	_arrl = _curr->child;

    if ( ! _arrl )
	return -1;

    rv = _node_print ( _arrl );
    if ( rv < 0 )
	return -1;

    _arrl = _arrl->next;

    return 0;
}

static int
json_select ( char *name, char *value )
{
    cJSON *node;

    if ( strcmp(name, "..") == 0 )
    {
	if ( _curr->parent )
	    _curr = _curr->parent;
    }
    else
    {
	node = cJSON_Lookup ( _curr->child, name );
	if ( ! node )
	    return -1;

	_curr = node;
    }

    return 0;
}

enum {
     CMD_BAD = -1
    ,CMD_ADD_INT
    ,CMD_ADD_STRING
    ,CMD_ADD_BOOL
    ,CMD_ADD_OBJECT
    ,CMD_CLOSE_OBJECT
    ,CMD_ADD_ARRAY
    ,CMD_CLOSE_ARRAY
    ,CMD_INIT
    ,CMD_DUMP
    ,CMD_DUMP_ROOT
    ,CMD_LOAD_FILE
    ,CMD_GET_VAR
    ,CMD_SELECT
};

struct {
    char *cmd;
    int value;
    int (*fn) (char *, char *);
} cmd_table[] = {
    { "add_int", CMD_ADD_INT, json_add_int },
    { "add_string", CMD_ADD_STRING, json_add_string },
    { "add_bool", CMD_ADD_BOOL, json_add_bool },
    { "add_object", CMD_ADD_OBJECT, json_add_object },
    { "close_object", CMD_CLOSE_OBJECT, json_close_object },
    { "add_array", CMD_ADD_ARRAY, json_add_array },
    { "close_array", CMD_CLOSE_ARRAY, json_close_array },
    { "init", CMD_INIT, json_init },
    { "dump", CMD_DUMP, json_dump },
    { "dump_root", CMD_DUMP_ROOT, json_dump_root },
    { "load_file", CMD_LOAD_FILE, json_load_file },
    { "get_var", CMD_GET_VAR, json_get_var },
    { "get_var_array", CMD_GET_VAR, json_get_var_array },
    { "select", CMD_SELECT, json_select },
};

#define CMDS	( sizeof(cmd_table) / sizeof(cmd_table[0]) )

/*
 *
 */
static int
json_do ( char *cmdstr, char *name, char *value )
{
    int rv, i;
    int (*fn) (char *, char *) = NULL;

    for ( i = 0; i < CMDS ; i++ ) {
	if ( strcmp(cmd_table[i].cmd, cmdstr) == 0 )
	    fn = cmd_table[i].fn;
    }

    if ( ! fn )
	return -1;

    _printbuf[1] = '\0';

    rv = fn ( name, value );

    return rv;
}

/*
 *
 */
static int
json_rx ( void )
{
    struct sockaddr_un saun_addr;
    struct sockaddr *peer_addr;
    socklen_t peer_addr_len;
    struct json_event e;
    int rv, len;

    peer_addr = (struct sockaddr *)&saun_addr;
    peer_addr_len = sizeof saun_addr;

    len = recvfrom ( json_sd, &e, sizeof e, 0, peer_addr, &peer_addr_len );
    if ( len <= 0 )
    {
	JSON_LOG ( "recvfrom:: %d:%s", errno, strerror(errno) );
	return len;
    }

    rv = json_do ( e.cmd, e.name, e.value );

    _printbuf[0] = (char)rv;

    rv = sendto ( json_sd, _printbuf, strlen(PRINTBUF) + 1, 0,
	    peer_addr, peer_addr_len );
    if ( rv < 0 )
	JSON_LOG ( "sendto:: %d:%s", errno, strerror(errno) );

    return len;
}

/*
 *
 */
static void
json_main ( void )
{
    fd_set readfds, readfds_org;
    int max_sd, rv;

    FD_ZERO ( &readfds_org );

    FD_SET ( json_sd, &readfds_org );

    max_sd = json_sd;

    while ( true )
    {
	readfds = readfds_org;

	rv = select ( max_sd + 1, &readfds, NULL, NULL, NULL );
	if ( rv < 0 )
	{
	    if ( errno != EINTR && errno != EAGAIN )
		JSON_LOG ( "select error, %d: %s\n", errno, strerror(errno) );
	}
	else
	{
	    if ( FD_ISSET(json_sd, &readfds) )
		json_rx ();
	    else
		JSON_LOG ( "select rv: %d", rv );
	}
    }
}

/*
 *
 */
static int
toss_tx ( struct json_event *e )
{
    struct sockaddr_un serv_addr;	/* server */
    struct sockaddr_un cli_addr;	/* client */
    int sd, rv, i;
#define MAX_TX_RETRY	3

    /* open UDS socket */
    sd = sock_uds_dgram_comm_client_init ( &serv_addr, UDS_FILE_SERVER,
	    &cli_addr, UDS_FILE_CLIENT );
    if ( sd < 0 )
    {
	JSON_LOG ( "sd: %d, errno: %d, %s", sd, errno, strerror(errno) );
	return -1;
    }

    for ( i = 0; i < MAX_TX_RETRY; i++ )
    {
	rv = sendto ( sd, e, sizeof(*e), 0,
	    (struct sockaddr *)&serv_addr, sizeof serv_addr );
	if ( rv > 0 )
	    break;

	usleep ( 50 * 1000 );
    }

    if ( i == MAX_TX_RETRY )
    {
	JSON_LOG ( "sendto:: errno: %d, %s", errno, strerror(errno) );
	goto done;
    }

    rv = recv_to ( sd, _printbuf, PRINT_BUFLEN, 0, 2 );
    if ( rv < 0 )
    {
	JSON_LOG ( "recv:: errno: %d, %s", errno, strerror(errno) );
	goto done;
    }

    _printbuf[rv] = '\0';

    rv = _printbuf[0];

    printf ( "%s", PRINTBUF );

done:
    close ( sd );

    return rv;
}

static int
_init ( void )
{
    int sd;

    sd = sock_uds_dgram_init ( &g_cjson_addr, UDS_FILE_SERVER, SOCK_FLAG_SERVER );
    if ( sd < 0 )
	return -1;

    json_sd = sd;

    json_init ( NULL, NULL );

    return 0;
}

static void
_deinit ( void )
{
    close ( json_sd );
}

#define USAGE_NEWLINE	"\n           "

void
usage ( void )
{
    fprintf ( stderr,
	"Usage: cjson COMMAND | "
	" [-"
		"r"
	"]"
	USAGE_NEWLINE
	" COMMAND: init, dump, add_int, add_string, ..."
	"\n"
    );

    exit ( 2 );
}

struct options {
    int read;
};

/*
 *
 */
static int
_opt_parser ( int argc, char **argv, struct options *o )
{
    const char *optstring = "r";
    int ch;	/* option */

    while ( (ch = getopt(argc, argv, optstring)) != EOF )
    {
	switch ( ch )
	{
	case 'r':
	    o->read = true;
	    break;

	default:
	    usage ();
	}
    }

    return 0;
}

int read_main ( char *myname, int argc, char **argv );

/*
 *
 */
int main ( int argc, char **argv, char **envp )
{
    int rv;
    struct options opt;

    memset ( &opt, 0, sizeof opt );

    //_opt_parser ( argc, argv, &opt );

    if ( argc > 1 )
    {
	struct json_event e;

	if ( opt.read )
	    read_main ( argv[0], argc - 2, &argv[2] );

	Strncpy ( e.cmd, argv[1], sizeof e.cmd );
	if ( argc > 2 )
	{
	    Strncpy ( e.name, argv[2], sizeof e.name );
	    if ( argc > 3 )
	    {
		Strncpy ( e.value, argv[3], sizeof e.value );
	    }
	    else
		e.value[0] = '\0';
	}
	else
	{
	    e.name[0] = '\0';
	    e.value[0] = '\0';
	}

	rv = toss_tx ( &e );
	if ( rv < 0 )
	    rv *= -1;

	exit ( rv );
    }

    rv = _init ();
    if ( rv < 0 )
	exit (2);

    //skeleton_daemon ();

    json_main ();

    _deinit ();

    return 0;
}

/*
 *
 */
int
read_main ( char *myname, int argc, char **argv )
{
    char *file, *name, *p;
    int rv;

    if ( argc < 2 )
	goto help;

    file = argv[0];
    name = argv[1];

    rv = json_load_file ( file, NULL );
    if ( rv < 0 )
    {
	printf ( "fail to load file: %s\n", file );
	exit (2);
    }

    while ( (p = strchr(name, '.')) )
    {
	*p = '\0';
	rv = json_select ( name, NULL );
	if ( rv < 0 )
	{
	    printf ( "not found: %s\n", name );
	    exit (2);
	}

	name = p + 1;
    }

    rv = json_get_var ( name, NULL );
    if ( rv < 0 )
    {
	printf ( "fail to get value for %s\n", name );
	exit (2);
    }

    printf ( "%s", PRINTBUF );

    exit (0);

help:
    printf ( "Usage: %s -r FILE NAME.PATH\n", myname );

    exit (255);
}

