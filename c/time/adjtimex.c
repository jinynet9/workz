#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/timex.h>

void ntp_adjtime_dump ( void )
{
    struct timex timex_info = {};
    timex_info.modes = 0;         /* explicitly don't adjust any time parameters */

    int ntp_result = ntp_adjtime(&timex_info);

    printf("\nnpt_adjtime()\n");
    printf("Max       error: %9ld (us)\n", timex_info.maxerror);
    printf("Estimated error: %9ld (us)\n", timex_info.esterror);
    printf("Clock precision: %9ld (us)\n", timex_info.precision);
    printf("Jitter:          %9ld (%s)\n", timex_info.jitter,
           (timex_info.status & STA_NANO) ? "ns" : "us");
    printf("Synchronized:    %9s\n", 
           (ntp_result >= 0 && ntp_result != TIME_ERROR) ? "yes" : "no");
}

/* Written for https://serverfault.com/questions/1077601/how-to-check-whether-the-system-time-is-synchronised-to-ntp-server-without-knowi */

void test_status(
    int st) 
{
    if (st & STA_PLL)
	printf("Phase locked loop\n");
    if (st & STA_PPSFREQ)
	printf("Pulse per second frequency discipline\n");
    if (st & STA_FLL)
	printf("PPS Time discipline\n");
    if (st & STA_INS)
	printf("Insert leap second and end-of-day\n");
    if (st & STA_DEL)
	printf("Delete leap second and end-of-day\n");
    if (st & STA_UNSYNC)
	printf("Clock is not syncronized\n");
    if (st & STA_FREQHOLD)
	printf("Hold frequency\n");
    if (st & STA_PPSSIGNAL)
	printf("Valid PPS signal is present\n");
    if (st & STA_PPSJITTER)
	printf("PPS signal jitter exceeded\n");
    if (st & STA_PPSWANDER)
	printf("PPS Signal wander exceeded\n");
    if (st & STA_PPSERROR)
	printf("PPS signal calibration error\n");
    if (st & STA_CLOCKERR)
	printf("Clock hardware fault\n");

    if (st & STA_NANO)
	printf("Nanosecond resolution\n");
    else
	printf("Microsecond resolution\n");

    if (st & STA_MODE)
	printf("Frequency locked loop\n");
    else
	printf("Phase locked loop\n");
}

int main() {
    struct timex tx = {};
    tx.modes = ADJ_OFFSET_SS_READ;
    int err = adjtimex(&tx);

    switch(err) {
    case -1:
	printf("Time error: %s\n", strerror(errno));
	break;

    case TIME_WAIT:
	printf("Leap second insert/delete completed\n");
	break;

    case TIME_INS:
	printf("Leap second to be added next UTC day\n");
	break;

    case TIME_DEL:
	printf("Leap second to be deleted next UTC day\n");
	break;

    case TIME_OOP:
	printf("Leap second insertion in progress\n");
	break;

    case TIME_ERROR:
	printf("Error getting time\n");
	break;

    case TIME_OK:
	printf("Time OK\n");
	break;

    default:
	printf("Time default: %x (%d)\n", err, err);
	break;
    }

    test_status(tx.status);

    ntp_adjtime_dump ();

    exit(0);
}
