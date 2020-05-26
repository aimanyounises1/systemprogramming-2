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
 void child(void)
{
    sleep(1);
    write(fd[0], &val, sizeof(val));  
    kill(getppid(), SIGUSR1); 
    while (1)
    {
	read(fd[0], &val, sizeof(val));
	if(val < 5){
		sleep(1); 
		val++;
		write(fd[0], &val, sizeof(val));
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
	
    	read(fd[1], &val, sizeof(val));
	
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
	pipe(fd);
    struct sigaction SA;
    sigemptyset(&SA.sa_mask);
    SA.sa_flags = 0;
    SA.sa_handler = fun;
    if (sigaction(SIGUSR1, &SA, NULL) == -1){
        printf("Failed");
	exit(2);
    }
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