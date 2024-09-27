#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//
//
//

void alarmHandler ( int signo )
{
    static int count = 0;

    count++;
    printf ( "Count: %d\n", count );
    fflush ( stdout );

    // alarm
    if ( count < 5 )
	alarm ( 2 );
    else
    {
	printf ( "Count: %d ... TIME OUT !\n", count );
	exit (0);
    }
}

int sigalm_main ( int argc, char *argv[] )
{
    signal ( SIGALRM, alarmHandler );

    alarm ( 2 );

    while (1)
    {
	printf ( "Now I'm in while statement\n" );
	sleep (1);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//

void (*org_sigint_handler)(int);

void sigint_handler( int signo )
{
    printf ( "Ctrl-C 키를 눌루셨죠!!\n" );
    printf ( "또 누르시면 종료됩니다.\n" );
    signal ( SIGINT, org_sigint_handler );   // 또는 signal( SIGINT, SIG_DFL);
}

int sigint_main ( int argc, char *argv[] )
{
    org_sigint_handler = signal ( SIGINT, sigint_handler );

    while ( 1 )
    {
	printf ( "Now I'm in while statement\n" );
	sleep (1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//
//

void timeout ( int sig )
{
    if ( sig == SIGALRM )
	puts ( "Time out!" );

    alarm ( 2 );
}

void keycontrol ( int sig )
{
    if ( sig == SIGINT )
	puts ( "CTRL+C pressed" );
}

int tk_main ( int argc, char *argv[] )
{
    int i;

    signal ( SIGALRM, timeout );
    signal ( SIGINT, keycontrol );

    alarm ( 2 );

    for ( i = 0; i < 3; i++ )
    {
	puts ( "wait... 100 secs" );
	sleep (100);
	puts ( "sleep end" );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//

int main ( int argc, char *argv[] )
{
#if 0
    sigalm_main ( argc, argv );
    sigint_main ( argc, argv );
#endif
    tk_main ( argc, argv );
}

