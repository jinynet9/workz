/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"

int main_create_objects ( int argc, char **argv, char **envp );
int main_file_parse ( int argc, char **argv, char **envp );

int main ( int argc, char **argv, char **envp )
{
#if 0
    main_create_objects ( argc, argv, envp );
    return 0;
#endif

    main_file_parse ( argc, argv, envp );

    return 0;
}


/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    size_t len = 0;

    /* formatted print */
    out = cJSON_Print(root);
    if ( ! out )
	return -1;

    printf("out: %s\n", out);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    free(out);
    free(buf);
    return 0;
}

int main_create_objects ( int argc, char **argv, char **envp )
{
    /* declare a few. */
    cJSON *root;
    cJSON *node;
    cJSON *status;
    cJSON *alarm;
    cJSON *cpu_load;
    cJSON *tjson;
    int rv;

    root = cJSON_CreateObject ();

    cJSON_AddStringToObject ( root, "message", "report" );
    cJSON_AddStringToObject ( root, "IMEI", "123456789" );

    status = cJSON_CreateObject ();

    cJSON_AddItemToObject ( root, "status", status );
    cJSON_AddStringToObject ( status, "model", "DSW1024XL" );
    cJSON_AddIntToObject ( status, "uptime", 7392186 );
    cJSON_AddBoolToObject ( status, "wlink", true );

    cJSON_AddIntToObject ( status, "rsrp", -95 );
    cJSON_AddInt64StrToObject ( status, "traffic_rx", 123456789123456789 );

    alarm = cJSON_CreateArray ();
    cJSON_AddItemToObject ( root, "alarm", alarm );
    cJSON_AddStringToArray ( alarm, "cpu" );
    cJSON_AddStringToArray ( alarm, "memory" );
    cJSON_AddStringToArray ( alarm, "eth0" );
    cJSON_AddStringToArray ( alarm, "wwan" );

    cpu_load = cJSON_CreateArray ();
    cJSON_AddItemToObject ( root, "cpu_load", cpu_load );
    cJSON_AddIntToArray ( cpu_load, 1 );
    cJSON_AddIntToArray ( cpu_load, 5 );
    cJSON_AddIntToArray ( cpu_load, 15 );

  {
    char *out;
    out = cJSON_Print(cpu_load->parent);
    printf("%s\n", out);
    free ( out );

    tjson = cJSON_Lookup ( root, ".status.model" );
    if ( tjson )
    {
	out = cJSON_Print(tjson);
	printf("%s\n", out);
	free ( out );
    }
    goto done;
  }

  {
    char *out = cJSON_Print(root);
    printf("%s\n", out);
    free ( out );
    cJSON_DebugDump ( root, 0 );
    goto done;
  }

    /* Print to text */
    rv = print_preallocated ( root );
    if ( rv < 0 )
	goto done;

done:
    cJSON_Destroy(root);

    return 0;
}

int main_file_parse ( int argc, char **argv, char **envp )
{
    cJSON *json;
    cJSON *tjson;
    char *file, *key_path;
    char *out;

    if ( argc < 3 )
    {
	printf ( "myname file object\n" );
	exit ( 1 );
    }

    file = argv[1];
    key_path = argv[2];

    json = cJSON_LoadFile ( file );
    if ( ! json )
    {
	printf ( "fail to parse: %s\n", file );
	exit ( 1 );
    }

    //cJSON_DebugDump ( json, 0 );

    tjson = cJSON_Lookup ( json, key_path );
    if ( tjson )
    {
	out = cJSON_Print(tjson);
	printf("%s\n", out);
	free ( out );
    }

    cJSON_Destroy ( json );

    return 0;
}

