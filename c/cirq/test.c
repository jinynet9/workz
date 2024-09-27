
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cirq.h"

void
_dump ( void *data, void *aux )
{
    int d = (int)data;

    printf ( "%d\n", d );
}

int main ( int argc, char **argv, char **envp )
{
    int i;
    int idx = 0;
    struct ring_buf_s *m;
    void *d;

    m = cirq_create ( 10 );
    if ( ! m )
    {
	printf ( "ring buffer creation failed\n" );
	return -1;
    }

    for ( i = 0; i < 43; i++ )
    {
	cirq_enque_force ( m, (void *)3 );
	cirq_enque_force ( m, (void *)5 );
	cirq_enque_force ( m, (void *)4 );
    }

    cirq_traverse ( m, _dump, NULL );

    CIRQ_LOOP ( m, i, d )
    {
	printf ( "%d: %d\n", i, (int)d );
    }

    cirq_dump ( m );

    return 0;
}
