
#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <unistd.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/wait.h>
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */


int main (int argc, char **argv){
    int i;                        /*      loop variables          */
    key_t shmkey;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    sem_t *sem;                   /*      synch semaphore         *//*shared */
    pid_t pid;                    /*      fork pid                */
    int *p;                       /*      shared variable         *//*shared */
    unsigned int n;               /*      fork count              */
    unsigned int value = 1;           /*      semaphore value         */
    char *sem_name = "pSem";

    /* initialize semaphores for shared processes */
    sem = sem_open (sem_name, O_CREAT | O_EXCL, 0644, value); 
    if ( sem == NULL )
    {
	perror ( "sem open error" );
	exit (1);
    }

    /* name of semaphore is "pSem", semaphore is reached using this name */
    sem_unlink (sem_name);      
    /* unlink prevents the semaphore existing forever */
    /* if a crash occurs during the execution         */
    printf ("semaphores initialized.\n\n");

    printf ("sem wait...\n");
    sem_wait (sem);           /* P operation */

    printf ("sleep...\n");
    sleep ( 10 );

    sem_close(sem);
}


int main2 (int argc, char **argv){
    int i;                        /*      loop variables          */
    key_t shmkey;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    sem_t *sem;                   /*      synch semaphore         *//*shared */
    pid_t pid;                    /*      fork pid                */
    int *p;                       /*      shared variable         *//*shared */
    unsigned int n;               /*      fork count              */
    unsigned int value;           /*      semaphore value         */

    /* initialize a shared variable in shared memory */
    shmkey = ftok ("/dev/null", 5);       /* valid directory name and a number */
    printf ("shmkey for p = %d\n", shmkey);
    shmid = shmget (shmkey, sizeof (int), 0644 | IPC_CREAT);
    if (shmid < 0){                           /* shared memory error check */
        perror ("shmget\n");
        exit (1);
    }

    p = (int *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
    *p = 0;
    printf ("p=%d is allocated in shared memory.\n\n", *p);

    /********************************************************/

    printf ("How many children do you want to fork?\n");
    printf ("Fork count: ");
    scanf ("%u", &n);

    printf ("What do you want the semaphore value to be?\n");
    printf ("Semaphore value: ");
    scanf ("%u", &value);

    /* initialize semaphores for shared processes */
    sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, value); 
    if ( sem == NULL )
    {
	perror ( "sem open error" );
	exit (1);
    }

    /* name of semaphore is "pSem", semaphore is reached using this name */
    sem_unlink ("pSem");      
    /* unlink prevents the semaphore existing forever */
    /* if a crash occurs during the execution         */
    printf ("semaphores initialized.\n\n");


    /* fork child processes */
    for (i = 0; i < n; i++){
        pid = fork ();
        if (pid < 0)              /* check for error      */
            printf ("Fork error.\n");
        else if (pid == 0)
            break;                  /* child processes */
    }


    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
    if (pid != 0){
        /* wait for all children to exit */
        while ((pid = waitpid (-1, NULL, 0)) > 0){
           if (errno == ECHILD)
                break;
        }

        printf ("\nParent: All children have exited.\n");

        /* shared memory detach */
        shmdt (p);
        shmctl (shmid, IPC_RMID, 0);

        /* cleanup semaphores */
        sem_unlink("pSem");
        sem_close(sem);

        exit (0);
    }

    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else{
        sem_wait (sem);           /* P operation */
        printf ("  Child(%d) is in critical section.\n", i);
        sleep (1);
        *p += i % 3;              /* increment *p by 0, 1 or 2 based on i */
        printf ("  Child(%d) new value of *p=%d.\n", i, *p);
        sem_post (sem);           /* V operation */
        exit (0);
    }
}


#if 0

#include <stdio.h>
#include <semaphore.h>

int cnt = 0;	//공유자원(shared variable)
static sem_t hsem;	//semaphore 객체 변수

void* Thread1(void *arg);
void* Thread2(void *arg);

int main(int argc, char *argv[]){
    pthread_t thread1;	//main에서는 2개의 스레드(thread1, thread2) 선언. 
    pthread_t thread2;

    if(sem_init(&hsem, 0, 1) < 0){	//사용하려고 하는 세마포어에 대해 초기화, local 프로세스에서만 semaphre사용, binary semaphore
	fprintf(stderr, "Semaphore Initialization Error\n");
	return 1;
    }
    pthread_create(&thread1, NULL, Thread1, NULL);
    pthread_create(&thread2, NULL, Thread2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("%d\n", cnt);
    sem_destroy(&hsem);

    return 0;
}

void* Thread1(void *arg){
    int i, tmp;
    for(i=0; i<1000; i++){
	sem_wait(&hsem);
	tmp = cnt;
	usleep(1000);
	cnt = tmp+1;
	sem_post(&hsem);
    }
    printf("Thread1 End\n");
    return NULL;
}

void* Thread2(void *arg){
    int i, tmp;
    for(i=0; i<1000; i++){
	sem_wait(&hsem);
	tmp = cnt;
	usleep(1000);
	cnt = tmp+1;
	sem_post(&hsem);
    }
    printf("Thread2 End\n");
    return NULL;
}

#endif


