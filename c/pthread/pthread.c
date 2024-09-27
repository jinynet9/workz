/*
 * https://www.joinc.co.kr/w/Site/Thread/Beginning/PthreadApiReference
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int wait_main ( int, char ** );
int tid_main ( int, char ** );

int main ( int argc, char **argv )
{
    wait_main ( argc, argv );
    //tid_main ( argc, argv );

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//
#define NUM_THREADS 2
 
/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
    int tid;
    double stuff;
    int ret;
} thread_data_t;
 
/* thread function */
void *thr_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    static int ret = 0;

    printf ( "[thread] id: %d:0, ret: %d\n", data->tid, ret );

    if ( data->tid == 0 )
	sleep (1);

    ret = data->tid + 1;
    data->ret = ret;

    printf ( "[thread] id: %d:1, ret: %d(0x%08x)\n", data->tid, ret, &ret );

    pthread_exit ( (void *)&ret );
}
 
int wait_main(int argc, char **argv)
{
    pthread_t thr[NUM_THREADS];
    int i, rv;
    void *ret;
    /* create a thread_data_t argument array */
    thread_data_t thr_data[NUM_THREADS];

    /* create threads */
    for ( i = 0; i < NUM_THREADS; ++i )
    {
	thr_data[i].tid = i;

	rv = pthread_create ( &thr[i], NULL, thr_func, &thr_data[i] );
	if ( rv )
	{
	    fprintf ( stderr, "error: pthread_create, rv: %d\n", rv );
	    return EXIT_FAILURE;
	}

	printf ( "Created new thread: %u\n", (unsigned int)thr[i] );
    }

    /* block until all threads complete */
    for ( i = 0; i < NUM_THREADS; ++i )
    {
	pthread_join ( thr[i], &ret );

	printf ( "[main] thread id %d ret: %d(0x%08x), data.ret: %d\n",
		thr_data[i].tid, *(int *)ret, ret, thr_data[i].ret );
    }

    return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//

void
printids ( const char *s )
{   
    pid_t pid;
    pthread_t tid;
    pid = getpid (); 
    tid = pthread_self (); 

    printf("%-15s pid %u tid %u (0x%x)\n", s, 
            (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

// Global variable:
int i = 2;

void *
foo ( void* p )
{
    printids ( "new thread:" );

    // Print value received as argument:
    printf ( "Value recevied as argument in starting routine: %i\n", *(int *)p );

    // Return reference to global variable:
    pthread_exit ( (void *)&i );
}

/*
 *
 */
int tid_main ( int argc, char **argv )
{
    // Declare variable for thread's ID:
    pthread_t id;

    int j = 1;
    pthread_create ( &id, NULL, foo, &j );
    printf ( "Created new thread: %u\n", (unsigned int)id );

    int *ptr;

    printids ( "main thread:" );

    // Wait for foo() and retrieve value in ptr;
    pthread_join ( id, (void**)&ptr );
    printf ( "Value recevied by parent from child: %i\n", *ptr );

    return 0;
}

#if 0
// 쓰레드 함수
// 1초를 기다린후 아규먼트^2 을 리턴한다.
void *
t_function(void *data)
{
    char a[100000];
    int num = *((int *)data);

    printf("Thread Start\n");
    sleep(5);
    printf("Thread end\n");

    return NULL;
}

int main()
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;

    printf("Before Thread\n"); 
    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 식별번호 p_thread 를 가지는 쓰레드를 detach 
    // 시켜준다.
    pthread_detach(p_thread);

    //pause();
    getchar();
    return 0;
}
#endif

#if 0
pthread_t threads[5];
int done[5];

void *thread_main(void *);

int main(void)
{
    int i;
    int rc;
    int status;

    printf("pid=%d\n", getpid());

    for (i = 0; i < 5; i++)
    {	
	done[i] = 0;
	pthread_create(&threads[i], NULL, &thread_main, (void *)i);
	printf("%d, %d\n", i, threads[i]);
    }

    for (i = 4; i >= 0; i--)
    {
	done[i] = 1;
    }

    /* 쓰레드들이 실행하고 종료할 때 까지 잠시 기다린다.
       */
    getchar();

    return 0;
}

void *thread_main(void *arg)
{
    int i, id;
    double result=0.0;

    pthread_detach(pthread_self()); /* 쓰레드 분리 */

    id = (int)arg;

    printf("therad %d started, pid:%d\n", id, getpid());

    while (!done[id])
    {
	for (i=0; i < 1000000; i++)
	{
	    result = result + (double)random();
	}
	printf("thread %d running, result = %e\n", id, result);
    }

    printf("thread %d terminated....\n", (int)arg);
    pthread_exit((void *) id);
}
#endif

#if 0
pthread_t threads[5];
int done[5];

void *thread_main(void *);

int main(void)
{
    int i;
    int rc;
    int status;

    printf("pid=%d\n", getpid());

    for (i = 0; i < 5; i++)
    {	
	done[i] = 0;
	pthread_create(&threads[i], NULL, &thread_main, (void *)i);
	printf("[main] pthread_create() %d, %d\n", i, threads[i]);
    }

    for (i = 4; i >= 0; i--)
    {
	done[i] = 1;
	rc = pthread_join(threads[i], (void **)&status); /* detach thread에서는 사용할 필요 없다. */
	if (rc == 0)
	{
	    printf("Completed join with thread %d status= %d\n", i, status);
	}
	else
	{
	    printf("ERROR; return code from pthread_join() is %d, thread %d\n", rc, i);
	    return -1;
	}
    }

    return 0;
}

void *thread_main(void *arg)
{
    int i, id;
    double result=0.0;

    pthread_detach(pthread_self()); /* 쓰레드 분리 */

    id = (int)arg;

    printf("therad: %d, pid:%d\n", id, getpid());

    while (!done[id])
    {
	for (i=0; i < 1000000; i++)
	{
	    result = result + (double)random();
	}
	printf("thread: %d, result = %e\n", id, result);
    }

    pthread_exit((void *) id);
}
#endif

#if 0
pthread_t threads[5];
int done[5];

void *thread_main(void *);

int main(void)
{
    int i;
    int rc;
    int status;

    printf("pid=%d\n", getpid());

    for (i = 0; i < 5; i++)
    {	
	done[i] = 0;
	pthread_create(&threads[i], NULL, &thread_main, (void *)i);
	printf("%d, %d\n", i, threads[i]);
    }

    for (i = 4; i >= 0; i--)
    {
	rc = pthread_cancel(threads[i]); // 강제종료
	if (rc == 0)
	{
	    // 자동종료
	    rc = pthread_join(threads[i], (void **)&status);
	    if (rc == 0)
	    {
		printf("Completed join with thread %d status= %d\n",i, status);
	    }
	    else
	    {
		printf("ERROR; return code from pthread_join() is %d, thread %d\n", rc, i);
		return -1;
	    }
	}
    }

    return 0;
}

void *thread_main(void *arg)
{
    int i, id;
    double result=0.0;

    id = (int)arg;

    printf("therad: %d, pid:%d\n", id, getpid());

    while (!done[id])
    {
	for (i=0; i < 1000000; i++)
	{
	    result = result + (double)random();
	}
	printf("thread: %d, result = %e\n", id, result);
    }

    pthread_exit((void *) id);
}
#endif

#if 0
pthread_t threads[5];
int done[5];

void *thread_main(void *);

int main(void)
{
    int i;
    int rc;
    int status;

    printf("pid=%d\n", getpid());

    for (i = 0; i < 5; i++)
    {	
	done[i] = 0;
	pthread_create(&threads[i], NULL, &thread_main, (void *)i);
	printf("%d, %ld\n", i, threads[i]);
    }

    for (i = 4; i >= 0; i--)
    {
	done[i] = 1;
	rc = pthread_join(threads[i], (void **)&status);
	if (rc == 0)
	{
	    printf("Completed join with thread %d status= %d\n",i, status);
	}
	else
	{
	    printf("ERROR; return code from pthread_join() is %d, thread %d\n", rc, i);
	    return -1;
	}
    }

    return 0;
}

void *thread_main(void *arg)
{
    int i, id;
    double result=0.0;

    id = (int)arg;

    printf("therad: %d, pid:%d\n", id, getpid());

    while (!done[id])
    {
	for (i=0; i < 1000000; i++)
	{
	    result = result + (double)random();
	}
	printf("thread: %d, result = %e\n", id, result);
    }

    pthread_exit((void *) id);
}
#endif

#if 0
void *
p_function(void *data)
{
    pid_t pid; //process id
    pthread_t tid; // thread id

    pid = getpid(); //4
    tid = pthread_self();

    char* thread_name = (char *)data;
    int i = 0;

    while(i<3)
    {
	printf("thread name : %s, tid : %x, pid : %u\n", thread_name, (unsigned int)tid, (unsigned int)pid); //5
	i++;
	sleep(1);
    }

    return NULL;
}

int
main ( int argc, char *argv[] )
{
    pthread_t pthread[2];
    int thr_id;
    int status;
    char p1[] = "thread_1";
    char p2[] = "thread_2";
    char p3[] = "thread_3";

    sleep(1); //1

    thr_id = pthread_create(&pthread[0], NULL, p_function, (void*)p1); //2
    if(thr_id < 0)
    {
	perror("pthread0 create error");
	exit(EXIT_FAILURE);
    }

    thr_id = pthread_create(&pthread[1], NULL, p_function, (void *)p2); //2
    if(thr_id < 0)
    {
	perror("pthread1 create error");
	exit(EXIT_FAILURE);
    }

    p_function((void *)p3); //3

    pthread_join(pthread[0], (void **)&status); //6
    pthread_join(pthread[1], (void **)&status);

    printf("end??\n");
    return 0;
}
#endif

