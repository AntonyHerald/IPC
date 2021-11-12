/* Demonstrate folk system call */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main() 
{
   pid_t newPid;
   int return_val = 1;
   int state;

   newPid = fork();
   if (newPid == -1)
   { //when process id is negative, there is an error, unable to fork
   	 printf("can't fork, error occured\n");
         exit(EXIT_FAILURE);
   } 
   else if (newPid == 0) 
   { //the child process is created
         printf("The child process is (%u)\n",getpid());
         
	 char * argv_list[] = {"ls","-lart","/home",NULL};
         execv("ls",argv_list); // the execv() only return if error occured.
         exit(0);
   } 
   else 
   { //for the parent process
      printf("The parent process is (%u)\n",getppid());
      
      if (waitpid(newPid, &state, 0) > 0) 
      { //wait untill the process change its state
          if (WIFEXITED(state) && !WEXITSTATUS(state))
            printf("program is executed successfully\n");
         
	  else if (WIFEXITED(state) && WEXITSTATUS(state)) 
	  {
            if (WEXITSTATUS(state) == 127) 
                printf("Execution failed\n");
	    else
               printf("program terminated with non-zero status\n");
         } 
	 else
            printf("program didn't terminate normally\n");
      }
      else 
      {
         printf("waitpid() function failed\n");
      }
      exit(0);
   }
   return 0;
}
