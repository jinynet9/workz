#ifndef __SYS_V_SEM__
#define __SYS_V_SEM__
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SEM_RESOURCE_MAX 1 // 이진 세마포어를 의미합니다.

/**
 * 아래를 이해하기 위해서는 다음을 이해해야 합니다.
 * struct sembuf { short sem_num; short sem_op; short sem_flg; };
 * - sem_num은 세마포어의 수로 여러 개의 세마포어를 사용하는 경우 사용합니다. 하나의 경우 0이 됩니다.
 * - sem_op에 값을 넣어주면 그 값을 세마포어 값에 더하거나 빼주도록 합니다.
 * - sem_flg는 세마포어 옵션으로 크게 2가지가 있습니다.
 *   - IPC_NOWAIT: POSIX에서 sem_trywait와 유사한 역할을 합니다.
 *   - SEM_UNDO: 프로세스가 세마포어를 돌려주지 않고 종료할 경우 커널이 알아서 증가시켜줍니다.
 */
#define SEM_LOCK {0, -1, SEM_UNDO} // P 연산에 해당합니다.
#define SEM_UNLOCK {0, 1, SEM_UNDO} // V 연상에 해당합니다.

union semun { /***** <<<<< UNION임을 유의하도록 합니다. >>>>> *****/
    int val;                   // SETVAL의 값을 지칭합니다.
    struct semid_ds *buf;      // IPC_SET, IPC_STAT을 위한 버퍼입니다.
    unsigned short int *array; // GETALL, SETALL을 위한 배열에 해당합니다.
};

static inline int
sem_init(int *semid, key_t key) {
    union semun semopts;
    // 1은 세마포어의 크기를 의미합니다. 
    if((*semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0666)) == -1) {
        return -1;
    }
    semopts.val = SEM_RESOURCE_MAX;
    /**
     * - 1st arg: 세마포어 아이디
     * - 2nd arg: 여러 개의 세마포어를 사용하는 경우 사용합니다. 하나의 경우 0이 됩니다.
     * - 3rd arg: 명령이 들어가는 곳으로 semun을 설정하도록 합니다.
     * - 4th arg: 설정이 될 semun에 해당합니다.
     */
    semctl(*semid, 0, SETVAL, semopts);
    return 0;
}

static inline int
sem_open(int *semid, key_t key) {
   // 세마포어의 접근만 하는 경우에는 0을 사용하도록 합니다.
   if((*semid = semget(key, 0, 0666)) == -1)  {
       perror("Semaphore doesn't exist");
       return -1;
   }
   return 0;
}

static inline void
sem_wait(int *semid) {
    struct sembuf sem_lock = SEM_LOCK;
    /**
     * int semop(int semid,. struct sembuf *sops, unsigned nsops);
     * 세마포어 식별자와 어떤 연산을 이루어 질 것인지와 관련된 명령입니다.
     * 여기서 nsops는 sops가 가리키는 배열의 수행 연산을 선택합니다.
     * SEM_LOCKS = {0, -1, SEM_UNDO} 이므로, 아래의 명령은 해당 배열에서 '-1'을 지칭하라는 것입니다.
     */
    semop(*semid, &sem_lock, 1);
}

static inline void
sem_post(int *semid) {
    struct sembuf sem_unlock = SEM_UNLOCK;
    semop(*semid, &sem_unlock, 1);
}

static inline void
sem_destroy(int *semid) {
    // 세마포어를 삭제하도록 합니다. 삭제이므로 semun은 0으로 아무것도 넘기지 않습니다. 
    semctl(*semid, 0, IPC_RMID, 0);
}
#endif


#if 0
#include <sys/types.h> 
#include <sys/sem.h> 
#include <sys/ipc.h> 
#include <stdio.h> 
#include <unistd.h> 

#define SEMKEY 12345 

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
};

static int  semid;
int main(int argc, char **argv)
{
    FILE* fp;
    char buf[11];
    char count[11];

    union semun sem_union; 

    // open 과 close 를 위한 sembuf 구조체를 정의한다. 
    struct sembuf mysem_open  = {0, -1, SEM_UNDO}; // 세마포어 얻기
    struct sembuf mysem_close = {0, 1, SEM_UNDO};  // 세마포어 돌려주기
    int sem_num;

    memset(buf, 0x00, 11);
    memset(count, 0x00, 11);

    // 아규먼트가 있으면 생성자
    // 그렇지 않으면 소비자이다.
    if (argc > 1)
        sem_num = 1;
    else 
        sem_num = 0;            

    // 세마포설정을 한다. 
    semid = semget((key_t)SEMKEY, sem_num, 0660|IPC_CREAT);
    if (semid == -1)
    {
        perror("semget error ");
        exit(0);
    }    

    // 세마포어 초기화
    sem_union.val = 1;
    if ( -1 == semctl( semid, 0, SETVAL, sem_union))
    {   
	printf( "semctl()-SETVAL 실행 오류\n");
	return -1; 
    }   

    // counter.txt 파일을 열기 위해서 세마포어검사를한다. 
    if(semop(semid, &mysem_open, 1) == -1)
    {
        perror("semop error ");
        exit(0);
    }

    if ((fp = fopen("counter.txt", "r+")) == NULL)
    {
        perror("fopen error ");
        exit(0);
    }
    // 파일의 내용을 읽은후 파일을 처음으로 되돌린다.  
    fgets(buf, 11, fp);
    rewind(fp);

    // 개행문자를 제거한다. 
    buf[strlen(buf) - 1] = 0x00;

    sprintf(count, "%d\n", atoi(buf) + 1); 
    printf("%s", count);
    // 10초를 잠들고 난후 count 를 파일에 쓴다. 
    sleep(10);
    fputs(count,fp);

    fclose(fp);
    // 모든 작업을 마쳤다면 세마포어 자원을 되될려준다
    semop(semid, &mysem_close, 1);
    return 1;
}
#endif

