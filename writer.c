#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define N 64
#define LEN (sizeof(SHM))

typedef struct shm
{
	char buf[N];
}SHM;

typedef union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
}SEMUN;

int main(int argc, char *argv[])
{
	int shmid;
	SHM *p;
	int semid;
	SEMUN myun;
	struct sembuf sems[2];

	key_t key = ftok(".", 'a');

	if (-1 == key)
	{
		perror("ftok error");
		exit(-1);
	}

	if (0 > (shmid = shmget(key, LEN, IPC_CREAT|IPC_EXCL|0666)))
	{
		if (EEXIST == errno)
		{
			shmid = shmget(key, LEN, 0666);
			p = (SHM *)shmat(shmid, NULL, 0);
		}
		else
		{
			perror("fail to shmget");
			exit(-1);
		}
	}
	else
	{
		p = (SHM *)shmat(shmid, NULL, 0);
	}

	if ( 0 > (semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0666)))
	{
		if (EEXIST == errno)
		{
			semid = semget(key, 2, 0666);
			myun.val = 0;
			semctl(semid, 0, SETVAL, myun);
			myun.val = 1;
			semctl(semid, 1, SETVAL, myun);
		}
		else
		{
			perror("fail to semget");
			exit(-1);
		}
	}
	else
	{
		myun.val = 0;
		semctl(semid, 0, SETVAL, myun);
		myun.val = 1;
		semctl(semid, 1, SETVAL, myun);
	}

	while (1)
	{
		sems[0].sem_num = 1;
		sems[0].sem_op = -1;
		sems[0].sem_flg = 0;
		semop(semid, sems, 1);
		printf("write to shm: ");
		//fgets(p->buf, LEN, stdin);
		strcpy(p->buf, "LED_ON");
		printf("%s\n", p->buf);
		sems[0].sem_num = 0;
		sems[0].sem_op = 1;
		sems[0].sem_flg = 0;
		semop(semid, sems, 1);
	}
	
	return 0;
}
