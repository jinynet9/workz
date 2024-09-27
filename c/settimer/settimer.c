#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

static void (*_cb) (void *aux) = NULL;
static void *_aux;

static void
timer_handler (int signum)
{
    _cb ( _aux );
}

int
timer_interval (
    int timer,		/* TIMER */
    int interval,	/* INTERVAL */
    void (*fn) (void *aux),
    void *aux
    )
{
    struct sigaction sa;
    struct itimerval itmr;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    /* Configure the itmr to timer after TIMER sec... */
    itmr.it_value.tv_sec = timer;
    itmr.it_value.tv_usec = 0;

    /* ... and every INTERVAL sec after that. */
    itmr.it_interval.tv_sec = interval;
    itmr.it_interval.tv_usec = 0;

    /* Start a virtual itmr. It counts down whenever this process is executing. */
    setitimer (ITIMER_VIRTUAL, &itmr, NULL);

    _cb = fn;
    _aux = aux;

    return 0;
}

void thandler ( void *aux )
{
    static int count = 0;
    printf("timer expired %d timers\n", ++count);
}

int main ()
{
    timer_interval ( 1, 1, thandler, NULL );

    /* Do busy work.  */
    while (1);

    return 0;
}

