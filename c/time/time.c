
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include <string.h>

#define HOURSECS	( 60 * 60 )
#define DAYSECS		( 24 * HOURSECS )

int days_main ( int argc, char **argv );
int ctime_main ( int argc, char **argv );
int test_main ( int argc, char **argv );
int time_valid_test_main ( int argc, char **argv );

int main ( int argc, char **argv )
{
    uint64_t imei=869751050106218;
    printf("imei: %lld\n", imei);

    days_main ( argc, argv );
return 0;

    ctime_main ( argc, argv );

    time_valid_test_main ( argc, argv );

    return 0;
}

int
days_main ( int argc, char **argv )
{
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    int days = tm->tm_yday + 1;

    printf("now: %ld, %d days\n", now, now / DAYSECS);
    printf("year day: %d\n", days);
}

int midnight() {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  int seconds_to_midnight = (60 * 60 * 24 - tm->tm_hour * 60 * 60 - tm->tm_min * 60 - tm->tm_sec);
  printf("There are %d seconds left until midnight.\n", seconds_to_midnight);
  return 0;
}
/*
 * gettimeofday
 */

/*
 *
 */
char *
time_sec_str ( int sec )
{
    int d, h, m, s;
    static char buf[80];

    m = sec / 60;
    s = sec % 60;

    h = m / 60;
    m = m % 60;

    d = h / 24;
    h = h % 24;

    if ( d > 0 )
	sprintf ( buf, "%d day(s) %02d:%02d:%02d", d, h, m, s );
    else
	sprintf ( buf, "%02d:%02d:%02d", h, m, s );

    return buf;
}

/*
 * 2 ->
 * 2:00 ->
 * 2:00:00 ->
 */
time_t
time_str2secs ( char *str )
{
    struct tm tm;
    time_t timex;
    char buf[256];

    memset ( &tm, 0, sizeof(struct tm) );

    sprintf ( buf, "1970-01-01 %s", str );

    strptime ( buf, "%Y-%m-%d %H:%M:%S", &tm );

    timex = mktime ( &tm );
    timex += tm.tm_gmtoff;

    return timex;
}

int
ctime_main ( int argc, char **argv )
{
    time_t utc_now;

    /* get current time */
    utc_now = time ( NULL );
    printf ( "%-20s: %ld: %s\n", "time now", utc_now, ctime(&utc_now) );

    exit (0);

    return 0;
}

/*
 * a.out 4:00 1:00
 */
void
time_test ( int argc, char **argv )
{
    time_t utc_now, lc_now, timex;
    time_t lc_mnight;		/* midnight */
    time_t gap_to_mnight;
    struct tm *ltm, tm;
    char buf[256];
    char *target = argv[1];
    char *during = argv[2];

    /* get current time */
    utc_now = time ( NULL );
    printf ( "%-20s: %ld: %s\n", "time now", utc_now, ctime(&utc_now) );

    ltm = localtime ( &utc_now );

    lc_now = utc_now + ltm->tm_gmtoff;

    lc_mnight = (lc_now / DAYSECS + 1) * DAYSECS;

    gap_to_mnight = lc_mnight - lc_now;
    printf ( "%-20s: %ld: %s\n", "gap to midnight",
	    gap_to_mnight, time_sec_str(gap_to_mnight) );

    timex = (gap_to_mnight + time_str2secs(target)) % DAYSECS;
    printf ( "%-20s: %ld: %s\n", "gap to target begin", timex, time_sec_str(timex) );

    timex = (timex + time_str2secs(during)) % DAYSECS;
    printf ( "%-20s: %ld: %s\n", "gap to target end", timex, time_sec_str(timex) );

    exit (0);
}

#if 0
format 문자	설명	예
%a	짧은 요일 이름	Thu
%A	긴 요일 이름	Thursday
%b, %h	짧은 달 이름	Jan
%B	긴 달 이름	January
%c	날짜 + 시간	Tue Apr  5 15:35:05 2022
%C	두자리 연도(00-99)	20
%d	그달의 일(01-31)	30
%D	날짜(MM/DD/YY)	04/05/22
%e	그 달의 일(1-31)	10
%H	그 날의 시(24시간)	23
%I	그 날의 시(12시간)	11
%m	달(01-12)	02
%M	분(00-59)	55
%p	오전/오후	PM
%S	초(00-60)	30
%T	%H:%M:%S	12:30:24
%R	%H:%M	12:30
%r	12시간 형식의 로컬 시간	05:40:12 PM
#endif

void
strptime_test ( int argc, char **argv )
{
    struct tm tm;
    time_t timex;
    char buf[256];

    memset ( &tm, 0, sizeof(struct tm) );
    strptime ( "2020-09-21 07:09:30", "%Y-%m-%d %H:%M:%S", &tm );
    timex = mktime ( &tm );
    printf ( "%ld: %s\n", timex, asctime(&tm) );

    memset ( &tm, 0, sizeof(struct tm) );
    strptime ( "1970-01-01 00:01", "%Y-%m-%d %H:%M:%S", &tm );
    timex = mktime ( &tm );
    timex += tm.tm_gmtoff;
    printf ( "%ld: %s\n", timex, asctime(&tm) );
printf("%d/%d/%d %d:%d:%d\n",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
                                        tm.tm_hour, tm.tm_min, tm.tm_sec);

    /*
     * strftime ()
     */
#if 0
    strftime ( buf, sizeof buf, "%a %T %Y-%m-%d", &tm );
    printf ( "%s\n", buf );
#endif

    exit (0);
}

int test_main ( int argc, char **argv )
{
    time_t utc_now, timex;	/* utc - raw time */
    time_t utc_mnight;		/* midnight */
    struct tm *ltm, *tmx;

//strptime_test ( argc, argv );

    if ( argc > 2 )
	time_test ( argc, argv );

    /*
     * get current time (time_t, utc epoch time)
     * time
     */
    utc_now = time ( NULL );
    //utc_now -= 2 * 60 * 60;
    printf ( "%-20s: %ld\n", "time now", utc_now );

    /*
     * time_t (utc epoch time) -> tm (local time)
     * localtime
     */
    ltm = localtime ( &utc_now );

    printf ( "%-20s: %02d:%02d:%02d, gmtoff(%d)\n", "localtime",
	    ltm->tm_hour, ltm->tm_min, ltm->tm_sec, ltm->tm_gmtoff );
    printf ( "%-20s: %s\n", "ctime", ctime ( &utc_now ) );
    printf ( "%-20s: %s\n", "asctime", asctime ( ltm ) );

    utc_mnight = (utc_now / DAYSECS + 1) * DAYSECS;
    printf ( "%-20s: %ld\n", "midnight", utc_mnight );

#if 0
    timex = (utc_mnight - utc_now + DAYSECS - ltm->tm_gmtoff) % DAYSECS;
#else
    timex = difftime ( utc_mnight, utc_now );
    timex = difftime ( timex, ltm->tm_gmtoff );
#endif
    printf ( "%-20s: %ld\n", "midnight remain", timex );

    /*
     * gmtime (time_t -> tm)
     */
    tmx = gmtime ( &timex );
    printf ( "%-20s: %02d:%02d:%02d\n", "gmtime",
	    tmx->tm_hour, tmx->tm_min, tmx->tm_sec );


    /*
     * mktime (tm -> time_t)
     */


    /*
     * difftime
     */
}

#if 0
현재 시간 정보 검색
time_t time(time_t *tloc);
 int gettimeofday(struct timeval *tp, void *tzp);

시간대 설정
void tzset(void);

시간 정보 분해
struct tm *localtime(const time_t *clock);
struct tm *gmtime(const time_t *clock);

초 단위 시간 생성
time_t mktime(struct tm *timeptr);

형식 지정 시간 출력
char *ctime(const time_t *clock);
char *asctime(const struct tm *tm);
size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr);
#endif

int time_isvalid ( char *str )
{
    struct tm tm;
    time_t timex;
    char buf[256];

    memset ( &tm, 0, sizeof(struct tm) );

    sprintf ( buf, "1970-01-01 %s", str );

    strptime ( buf, "%Y-%m-%d %H:%M:%S", &tm );

    timex = mktime ( &tm );

    return timex >= 0 ? true : false;
}

int time_valid_test_main ( int argc, char **argv )
{
    char *str = argv[1];
    int rv;

    if ( argc < 2 )
    {
	printf ( "usage: %s time-string\n", argv[0] );
	return 0;
    }

    rv = time_isvalid ( str );
    printf ( "%s : %s\n", str, rv == true ? "valid" : "invalid" );

    return 0;
}

