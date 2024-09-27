/*
 *
 */

#ifndef _CJSON_LOG_H_
#define _CJSON_LOG_H_

#if 0
#define JSON_DBG(fmt, arg...)					\
    zlog_err ( ZLOG_DEBUG, "CJSON:%s():: "fmt, __func__, ##arg )

#define JSON_LOG(fmt, arg...)					\
    zlog_err ( ZLOG_ADMIN, "CJSON: "fmt, ##arg )
#else
#define JSON_DBG(fmt, arg...)						\
  do {									\
    printf ( "CJSON:%s():: "fmt"\n", __func__, ##arg );			\
  } while (0)

#define JSON_LOG(fmt, arg...)						\
  do {									\
    printf ( "CJSON: "fmt"\n", ##arg );					\
  } while (0)
#endif

#endif /* _CJSON_LOG_H_ */

