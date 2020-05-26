#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
cat /proc/"process pid"/status | grep SigCgt
*/

int fd[2];
int val = 0;
sig_atomic_t sig_num;
 int counter;
 void parent(pid_t pid);
 void child()
{
    sleep(1);
    write(fd[1], &val, sizeof(val));  
    kill(getppid(), SIGUSR1); 
    while (1)
    {
	read(fd[0], &val, sizeof(val));
	if(val < 5){
		sleep(1); 
		val++;
		write(fd[1], &val, sizeof(val));
		kill(getppid(), SIGUSR1); 
	}
	else{ 
	   printf("Parent is going to be terminated\n");
	   break;
	}
    }
    
    close(fd[0]);
}
 void fun(int sig)
{
    sig_num = sig;
}
 void parent(pid_t pid)
{
	
    while(1){
    	read(fd[0], &val, sizeof(val));
    	printf(" 	%d\n",val);
    	if(val < 5){
		sleep(1);
		val++;
		write(fd[1], &val, sizeof(val));
    	kill(pid, SIGUSR1);
    	}
    	else{
    		printf("Child is going to be terminated\n");
		    write(fd[1], &val, sizeof(val));
			kill(pid, SIGUSR1);
		    
		break;
    	}
    }
   close(fd[1]);
}

int main()
{
	pipe(fd);// pipe fd[0] to read and fd[1] to write
     struct sigaction SA;
    sigemptyset(&SA.sa_mask);// setting the signal mask to empty 
    SA.sa_flags = 0;
    SA.sa_handler = fun;// pointer to the catch function 
    if (sigaction(SIGUSR1, &SA, NULL) == -1){
        printf("Failed");
	exit(2);
    }
	// startring chiled process
    pid_t pid = fork();
    if (pid < 0){
        printf("Fail");
	exit(2);
    }
    else if (pid == 0)
        child();
    else
        parent(pid);

    return 0;
}