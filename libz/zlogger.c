/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <syslog.h>

#include "zlogger.h"

static int g_zlevel = ZLOG_LEVEL_DEFAULT;

int
zlog_level_set ( int zlevel )
{
    g_zlevel = zlevel;

    return 0;
}

int
zlog ( int zlevel, int level, char *fmt, ... )
{
    va_list ap;
    char buf[4 * 1024];

    if ( zlevel > g_zlevel )
	return 0;

    if ( ! fmt )
	return -1;

    va_start ( ap, fmt );

    vsnprintf ( buf, sizeof buf, fmt, ap );

    va_end ( ap );

    syslog ( LOG_LOCAL7 | level, "%s", buf );

    return 0;
}

