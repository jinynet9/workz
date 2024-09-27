
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>	/* itimerval */

struct user_data {
    int data;
};

void
timer_handler ( int signo, siginfo_t *si, void *uc )
{
    time_t utc_now;
    struct user_data *data = si->_sifields._rt.si_sigval.sival_ptr;

    utc_now = time ( NULL );
    printf ( "signo: %d, data: %d, %s\n",
	    signo, ++data->data, ctime ( &utc_now ) );
}

int
timer_make (
    timer_t *timer_id,
    unsigned int sec,
    unsigned int msec,
    void (*handler) (int, siginfo_t *, void *),
    void *user_data
    )
{
    struct sigevent         se;
    struct itimerspec       its;
    struct sigaction        sa;
    static int next_sig;
    int signo, rv;
    unsigned int ns;

    if ( next_sig == 0 )
	next_sig = SIGRTMIN;

    if ( next_sig > SIGRTMAX )
	return -1;

    signo = next_sig++;

    /* specifz signal and handler. */
    memset ( &sa, 0, sizeof sa );
    sa.sa_flags = 0;
    sa.sa_flags |= SA_RESTART;
    sa.sa_flags |= SA_SIGINFO;
    sa.sa_sigaction = handler;     /* callback function */

    /* initialize signal */
    sigemptyset ( &sa.sa_mask );

    /* register signal handler */
    rv = sigaction ( signo, &sa, NULL );
    if ( rv < 0 )
    {
        printf ( "sigaction error: %s\n", strerror(errno) );
        return -1;
    }

    /* enable alarm */
    memset ( &se, 0, sizeof se );
    se.sigev_notify = SIGEV_SIGNAL;
    se.sigev_signo = signo;
    se.sigev_value.sival_ptr = user_data;

    /* create timer */
    rv = timer_create ( CLOCK_REALTIME, &se, timer_id );
    if ( rv < 0 )
    {
        printf ( "timer_create error: %s\n", strerror(errno) );
        return -1;
    }

    /* specify start delay and interval */
    memset ( &its, 0, sizeof its );
    ns = msec * 1000000;
    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = ns;
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = ns;

    /* start timer */
    rv = timer_settime ( *timer_id, 0, &its, NULL );
    if ( rv < 0 )
    {
        printf ( "timer_settime error: %s\n", strerror(errno) );
	timer_delete ( *timer_id );
        return -1;
    }

    return 0;
}

int read_stdin ( void );

int main()
{
    timer_t timer_id1, timer_id2;
    char buf[80];
    int nread;
    struct user_data data1 = { .data = 0 };
    struct user_data data2 = { .data = 0 };

    printf("SIGRTMIN: %d\n", SIGRTMIN );
    printf("SIGRTMAX: %d\n", SIGRTMAX );

    timer_make ( &timer_id1, 2, 0, timer_handler, &data1 );
    timer_make ( &timer_id2, 3, 0, timer_handler, &data2 );

    //signal ( SIGALRM, SIG_IGN );

    while ( 1 )
    {
#if 0
	printf("read start\n");
	nread = read(STDIN_FILENO, buf, sizeof(buf));
	printf("read return, nread: %d\n", nread);
	if ( nread < 0 )
	    printf("%d, %s\n", errno, strerror(errno));
#else
	read_stdin ();
#endif
    }

    timer_delete ( timer_id1 );
    timer_delete ( timer_id2 );
}



#if 0
void
timer_handler ( int signo )
{
    time_t utc_now;
    utc_now = time ( NULL );
    printf ( "signo: %d   %s\n", signo, ctime ( &utc_now ) );
}

int
timer_make (
    timer_t *timer_id,
    unsigned int sec,
    unsigned int msec,
    void (*handler) (int)
    )
{
    struct sigevent         se;
    struct itimerspec       its;
    struct sigaction        sa;
    static int next_sig;
    int signo, rv;
    unsigned int ns;

    if ( next_sig == 0 )
	next_sig = SIGRTMIN;

    if ( next_sig > SIGRTMAX )
	return -1;

    signo = next_sig++;

    /* specifz signal and handler. */
    memset ( &sa, 0, sizeof sa );
    sa.sa_flags = 0;
    sa.sa_flags |= SA_RESTART;
    sa.sa_handler = handler;     /* callback function */

    /* initialize signal */
    sigemptyset ( &sa.sa_mask );

    /* register signal handler */
    rv = sigaction ( signo, &sa, NULL );
    if ( rv < 0 )
    {
        printf ( "sigaction error: %s\n", strerror(errno) );
        return -1;
    }

    /* enable alarm */
    memset ( &se, 0, sizeof se );
    se.sigev_notify = SIGEV_SIGNAL;
    se.sigev_signo = signo;

    /* create timer */
    rv = timer_create ( CLOCK_REALTIME, &se, timer_id );
    if ( rv < 0 )
    {
        printf ( "timer_create error: %s\n", strerror(errno) );
        return -1;
    }

    /* specify start delay and interval */
    memset ( &its, 0, sizeof its );
    ns = msec * 1000000;
    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = ns;
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = ns;

    /* start timer */
    rv = timer_settime ( *timer_id, 0, &its, NULL );
    if ( rv < 0 )
    {
        printf ( "timer_settime error: %s\n", strerror(errno) );
        return -1;
    }

    return 0;
}

int
timer_make2 (
    void (*handler) (int),
    int sec,
    int msec
    )
{
    struct sigaction sa;
    struct itimerval itv;
    int signo = SIGTSTP, rv;

    memset ( &sa, 0, sizeof sa );
    sa.sa_handler = handler;     /* callback function */

    rv = sigaction ( signo, &sa, NULL );
    if ( rv < 0 )
    {
        printf ( "sigaction error: %s\n", strerror(errno) );
        return -1;
    }

    memset ( &itv, 0, sizeof itv );
    itv.it_value.tv_sec = sec;
    itv.it_value.tv_usec = msec * 1000000;
    itv.it_interval.tv_sec = sec;
    itv.it_interval.tv_usec = msec * 1000000;

    /* start a timer */
    setitimer ( ITIMER_REAL, &itv, NULL );

    return 0;
}

int main()
{
    timer_t timer_id1, timer_id2;
    char buf[80];
    int nread;

    // 타이머를 만든다
#if 1
    timer_make ( &timer_id1, 2, 0, timer_handler );
    timer_make ( &timer_id2, 3, 0, timer_handler );
#else
    timer_make2 ( timer_handler, 2, 0 );
#endif

    //signal ( SIGALRM, SIG_IGN );

    while ( 1 )
    {
	printf("read start\n");
	nread = read(STDIN_FILENO, buf, sizeof(buf));
	printf("read return, nread: %d\n", nread);
	if ( nread < 0 )
	    printf("%d, %s\n", errno, strerror(errno));
    }

    timer_delete ( timer_id1 );
    timer_delete ( timer_id2 );

}
#endif

#define TIMEOUT 5
#define BUF_LEN 1024
 
int read_stdin ( void )
{
    struct timeval tv;
    fd_set readfds;
    int ret;
    
    // 표준 입력에서 입력을 기다리기 위한 준비를 합니다.
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    // select가 5초 동안 기다리도록 timeval 구조체를 설정합니다.
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    
    // select() 시스템콜을 이용해 입력을 기다립니다.
    ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    
    if (ret == -1) {
        perror("select");
        return 1;
    }
    else if (!ret){
        printf("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }
    
    // select() 시스템콜이 양수를 반환했다면 '블록(block)'없이 즉시 읽기가 가능합니다.
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[BUF_LEN + 1];
        int len;
        
        // '블록(block)'없이 읽기가 가능합니다.
        len = read(STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) return 1;
        if (len) {
            buf[len] = '\0';
            printf("read: %s\n", buf);
        }
        
        return 0;
    }

    return 0;
}
