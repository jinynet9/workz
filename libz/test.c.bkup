/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "zut.h"
#include "zlogger.h"
#include "zinet.h"

#define TUCI_NMS_STATUS		""
#define TUCI_NMS_RX_MESSAGE	TUCI_NMS_STATUS"rx_message"
#define TUCI_NMS_RX_RESULT	TUCI_NMS_STATUS"rx_result"
#define TUCI_NMS_RX_INTERVAL	TUCI_NMS_STATUS"rx_interval"
#define TUCI_NMS_RESULT		TUCI_NMS_STATUS"result"
#define TUCI_NMS_REASON		TUCI_NMS_STATUS"reason"
#define TUCI_NMS_RECUR		TUCI_NMS_STATUS"recur"
#define TUCI_NMS_ONLINE		TUCI_NMS_STATUS"online"
#define TUCI_NMS_ALMMASK	TUCI_NMS_STATUS"almmask"
#define TUCI_NMS_CONFIG_PROFILE	TUCI_NMS_STATUS"config_profile"
#define TUCI_NMS_FOTA_RUN	TUCI_NMS_STATUS"fota_run"

#define SCONF_SH		"./sconf.sh"

#define TUCI_PATH		"."
#define TUCI_FILE		TUCI_PATH"/nms_status"
#define TUCI			SCONF_SH" "TUCI_FILE" "

#define TUCI_GET(N, V)							\
    do {								\
	V[0] = '\0';							\
	shell_read_tnl ( V, sizeof(V), TUCI" get %s", N ); 		\
    } while (0)

int main ( int argc, char **argv, char **envp )
{
    char rx_message[40], rx_result[80], interval[40], config_profile[40];
    char result[160];

    TUCI_GET ( TUCI_NMS_RX_MESSAGE, rx_message );
    TUCI_GET ( TUCI_NMS_RX_RESULT, rx_result );
    TUCI_GET ( TUCI_NMS_RESULT, result );

    printf("rx_message[%s]\n\n", rx_message);
    printf("rx_result[%s]\n\n", rx_result);
    printf("result[%s]\n\n", result);
    printf("bye\n");

    return 0;
}
