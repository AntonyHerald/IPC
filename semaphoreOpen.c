#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
//below headers are IPC related
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 256

char buf[SIZE];
sem_t *semID;

int main(int argc, char **argv)
{
	//sem_open()  creates a new POSIX semaphore or opens an existing semaphore.

	getcwd(buf, SIZE);
	printf("The LocalServ Path %s\n",buf);
	
	//semID  = sem_open(buf, O_CREAT|O_EXCL, 0644,sizeof(buf));
	semID = sem_open(buf, O_CREAT, 0644, SIZE);

	if(semID == SEM_FAILED){
		perror("Failed to Open Semaphore for semID \n");
		printf("Failed %u \n",semID);
		exit(1);
	}
	else
		printf("Semaphore Successful \n");


	return 0;
}

