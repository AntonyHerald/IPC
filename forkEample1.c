#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	printf("Im here %d\n", getpid() );
	

	pid_t pid = fork();   //if fork succeed it returns 0 and check is spwan and pid is assigned
	printf("The fork returned %d \n", pid);

	if (pid < 0 ) {
		printf("fork failed \n");
	}

	if(pid == 0 ){
		//Everything done inside this loop is within child process block
		
		printf("fork succeeded : Child process spwaned %d \n", getpid());
		sleep(5); // sleep for 5 sec
		printf("Exiting child process .... \n");
		exit(0);
	} //end of child-block

	printf("Im the parent, waiting for the child to complete .... \n");
	wait(NULL);

	printf("Parent Ended .... \n");
	
	return 0;
}






