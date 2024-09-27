/*
 * make hello
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define DIM(v)			( sizeof(v)/sizeof(v[0]) )

#define FLAG_SET(v,n)          do { (v) |= (n); } while (0)
#define FLAG_CLR(v,n)          do { (v) &= ~(n); } while (0)
#define FLAG_ISSET(v,n)        ( (v) & (n) ? 1 : 0 )


int time_main ( int argc, char **argv, char **envp );

int main ( int argc, char **argv, char **envp )
{
    time_main ( argc, argv, envp );

    return 0;
}

#include <regex.h>

int
_time_str_isvalid ( char *str )
{
    // Regex to check valid time in 24-hour format
    const regex pattern("([01]?[0-9]|2[0-3]):[0-5][0-9]");

    // If the time in 24-hour format
    // is empty return false
    if (str.empty())
    {
	return false;
    }

    // Return true if the time in 24-hour format
    // matched the ReGex
    if(regex_match(str, pattern))
    {
	return true;
    }
    else
    {
	return false;
    }
}

int time_main ( int argc, char **argv, char **envp )
{

    return 0;
}

