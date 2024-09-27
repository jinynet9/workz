#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#if 0
(gdb) p *(recursive_mutex_t *)  _hsl_lap_mutex
$2 = {mutex = {__data = {__lock = 0, __count = 0, __owner = 0, __nusers = 0, 
      __kind = 1, __spins = 0, __list = {__prev = 0x0, __next = 0x0}}, 
    __size = '\000' <repeats 19 times>, "\001", '\000' <repeats 27 times>, 
    __align = 0}, desc = 0x2d7f078 "Local ARP Proxy"}
#endif

/*
 *
 */
void
_mutex_dump ( pthread_mutex_t *m, char *title )
{
    if ( title )
	printf ( "\n%s:\n", title);

    printf ( "__data\n" );
    printf ( "  __lock   = %d\n", m->__data.__lock );
    printf ( "  __count  = %d\n", m->__data.__count );
    printf ( "  __owner  = %d\n", m->__data.__owner );
    printf ( "  __nusers = %d\n", m->__data.__nusers );
    printf ( "  __kind   = %d\n", m->__data.__kind );
    printf ( "  __spins  = %d\n", m->__data.__spins );
    printf ( "__size = %s\n", m->__size );
}

/*
 *
 */
int main ( int argc, char **argv, char **envp )
{
    pthread_mutex_t m;

    pthread_mutex_init ( &m, NULL );

    _mutex_dump ( &m, "init" );

    pthread_mutex_lock ( &m );

    _mutex_dump ( &m, "lock" );

    pthread_mutex_unlock ( &m );

    _mutex_dump ( &m, "unlock" );

    return 0;
}

#if 0
// 뮤텍스 객체 선언
pthread_mutex_t mutex_lock;
 
int g_count = 0;  // 쓰레드 공유자원.
 
void *t_function(void *data)
{
    int i;
    char* thread_name = (char*)data;
    
    
 
    pthread_mutex_lock(&mutex_lock);
 
    // critical section
    g_count = 0;   // 쓰레드마다 0부터 시작.
    for (i = 0; i < 3; i++)
    {
        printf("%s COUNT %d\n", thread_name, g_count);
        g_count++;  // 쓰레드 공유자원
        sleep(1);
    }
 
    pthread_mutex_unlock(&mutex_lock);
}
int main()
{
    pthread_t p_thread1, p_thread2;
    int status;
    // 뮤텍스 객체 초기화, 기본 특성으로 초기화 했음
    pthread_mutex_init(&mutex_lock, NULL);
 
    pthread_create(&p_thread1, NULL, t_function, (void *)"Thread1");
    pthread_create(&p_thread2, NULL, t_function, (void *)"Thread2");
 
    pthread_join(p_thread1, (void *)&status);
    pthread_join(p_thread2, (void *)&status);
}
#endif

