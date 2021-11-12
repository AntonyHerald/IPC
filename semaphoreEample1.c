#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
//below headers are IPC related
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>


int main()
{
	int i;
	key_t shmkey;
	int shmID;
	sem_t *sem;
	pid_t pid;
	int *p;
	unsigned int n;
	unsigned int value;

	/* creating a sharedkey */
	shmkey = ftok("//home/anto/Anto-Project/IPC/", 5);
	printf("The semephore key = key %d \n", shmkey);
	shmID = shmget(shmkey,sizeof(int), 0644|IPC_CREAT);

	if(shmID < 0 ) { //sharedMem key will always be positve number (< lessthan failure)
		perror("shmget failed: ");
		exit(1);
	}

	p = (int*) shmat (shmID,NULL,0);  //shmat = sharedmemory attach
	*p = 0;
	printf("p = (%d)  is allcated in the shared memory \n", *p);

	/*----------- Input the kind of semaphore or count required ----*/
	printf("Enter the number of child process you want fork() \n");
	scanf("%u",&n);

	printf("Enter Semaphore value (Binary or Counting to be decided)  \n");
	scanf("%u",&value);

	/*----------- Semaphore initialization  ----*/
	sem = sem_open("Semphore-Here", O_CREAT|O_EXCL, 0644,value); //Create semaphore
	//sem = sem_open("Semphore-Here", O_CREAT, 0644,value);    
	sem_unlink("Semephore-Here");      //make unlink call, to avoid link fails, as to link fresh
	printf("Semaphore Initialized \n");

	/*----------- Creation of child process (fork) ------*/
	for(i = 0 ; i<n; i++){
		pid = fork();
		
		if(pid < 0 ) 
			printf("Fork failed \n");
		else if (pid == 0)
			break;
	}//end of for loop

	if (pid != 0) {
		while (pid = waitpid(-1, NULL, 0)){
			if (errno == ECHILD)
			break;
		}
		printf("Parent: All children have exited \n");
	
		//Shared memory has to be detached after all work 
		shmdt(p); 
		shmctl(shmID, IPC_RMID, 0);

		//cleaning up stuff 
		sem_close(sem);
		sem_destroy(sem);
		printf("Belfore exit(0)\n");
		exit(0);
	}
	else{
		//Critical section operation done below
		sem_wait(sem);
		printf("Child (%d) is in critial section \n", i);
		sleep(1);

		*p += i%3;
		printf("Child (%d) new value of *p=%d \n", i, *p);
		sem_post(sem);

		exit(0);
	}
	sem_close(sem);
}



