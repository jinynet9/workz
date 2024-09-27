#include <stdio.h>

int main ( int argc, char **argv, char **envp )
{
    int i;

    for ( i = 0; envp[i]; i++ )
    {
        printf ( "%s\n", envp[i] );
    }

#if 0
    char **env;

    for ( env = envp; *env != 0; env++ )
    {
        printf ( "%s\n", *env );
    }
#endif

#if 0
    char **env;

    for ( env = envp; *env != 0; env++ )
    {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);
    }
#endif

    return 0;
}

