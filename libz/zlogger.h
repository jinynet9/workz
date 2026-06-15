
/*
 *
 */

#ifndef _ZUT_LOGGER_H_
#define _ZUT_LOGGER_H_

#include <syslog.h>

/* zlog level */
#define ZLOG_ADMIN	0
#define ZLOG_ERROR	1
#define ZLOG_WARNING	2
#define ZLOG_DEBUG	3

#define ZLOG_LEVEL_DEFAULT	ZLOG_WARNING

#define zlog_debug(ZLEVEL, fmt, arg...)		\
    zlog ( ZLEVEL, LOG_DEBUG, fmt, ##arg )

#define zlog_err(ZLEVEL, fmt, arg...)		\
    zlog ( ZLEVEL, LOG_ERR, fmt, ##arg )

#define zlog_crit(ZLEVEL, fmt, arg...)		\
    zlog ( ZLEVEL, LOG_CRIT, fmt, ##arg )

int zlog_level_set ( int zlevel );
int zlog ( int zlevel, int level, char *fmt, ... );

/*
LOG_EMERG
LOG_ALERT
LOG_CRIT
LOG_ERR
LOG_WARNING
LOG_NOTICE
LOG_INFO
LOG_DEBUG
*/

#endif /* _ZUT_LOGGER_H_ */
