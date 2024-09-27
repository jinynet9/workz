#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int ncount;    // 쓰레드간 공유되는 자원
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER; // 쓰레드 초기화

// 쓰레드 함 수 1
void* do_loop(void *data)
{
    int i;

    for (i = 0; i < 10; i++)
    {
	pthread_mutex_lock(&mutex); // 잠금을 생성한다.

        printf("loop1 : %d\n", ++ncount);

	pthread_mutex_unlock(&mutex); // 잠금을 해제한다.

	usleep(random() % 1000);
    }
}

// 쓰레드 함수 2
void* do_loop2(void *data)
{
    int i;

    for (i = 0; i < 10; i++)
    {
	pthread_mutex_lock(&mutex); // 잠금을 생성한다.

        printf("loop2 : %d\n", ++ncount);

	pthread_mutex_unlock(&mutex); // 잠금을 해제한다.

	usleep(random() % 1000);
    }
}    

int main()
{
    int thr_id;
    pthread_t p_thread[2];
    int status;
    int a = 1;

    ncount = 0;
    thr_id = pthread_create(&p_thread[0], NULL, do_loop, (void *)&a);
    thr_id = pthread_create(&p_thread[1], NULL, do_loop2, (void *)&a);

    pthread_join(p_thread[0], (void *) &status);
    pthread_join(p_thread[1], (void *) &status);

    status = pthread_mutex_destroy(&mutex);
    printf("code  =  %d\n", status);
    printf("programing is end\n");
    return 0;
}

#if 0
int sum;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *sum1(void* num)
{
    int n=*((int *)num);
    int i;

    for (i=0;i<n;i++){
	pthread_mutex_lock(&mutex);
	sum++;
	printf("sum1:%d\n",sum);
	pthread_mutex_unlock(&mutex);
	usleep(random() % 1000);
    }

    return NULL;
}

void *sum2(void *num)
{
    int n=*((int*)num);
    int i;

    for(i=0;i<n;i++){
	pthread_mutex_lock(&mutex);
	sum++;
	printf("sum2:%d\n",sum);
	pthread_mutex_unlock(&mutex);
	usleep(random() % 1000);
    }

    return NULL;
}

int main(){
    int thread;
    pthread_t p_thread[2];
    int sts;
    int arg;
    sum=0;
    arg=5;

    thread=pthread_create(&p_thread[0],NULL,sum1,(void *)&arg);
    thread=pthread_create(&p_thread[1],NULL,sum2,(void *)&arg);

    printf("main join 1 start\n");
    pthread_join(p_thread[0],(void**)&sts);
    printf("main join 1 end\n");
    printf("main join 2 start\n");
    pthread_join(p_thread[1],(void**)&sts);
    printf("main join 2 end\n");

    sts=pthread_mutex_destroy(&mutex);

    return 0;

}
#endif

