
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int fd[2];
int val=0;
int toFinish=1;
int x;
pid_t pid,ppid,id;

//fd[0]= to read
//fd[1]= to write


void handler1(int sig){
        if(val==5){
            
            toFinish = 0;
            return;
        }
        fflush(stdout);
            printf("%d",val);
            x = 0;
            read(fd[0], &x, sizeof(int));
            val = ++x;
            //close(fd[0]);
            write(fd[1], &val, sizeof(int));
          //  close(fd[1]);
          //  usleep(1000000);
            kill(pid, SIGUSR1); 

}
void handler2(int sig){
    if(val==5){
        fflush(stdout);
            printf("%d\n", val);
            toFinish = 0;
            return;
        }
        x =0;
        read(fd[0], &x, sizeof(int));
        val = ++x;
        //close(fd[0]);
         write(fd[1], &val, sizeof(int));
       // close(fd[1]);
          
}
int main(){
    pipe(fd);
    pid = fork();
    printf("%d",pid);
    signal(SIGUSR1,handler1);
    if(pid==0){
    signal(SIGUSR1,handler2);
    kill(getppid(),SIGUSR1); 
    }  while(1);
   close(fd[0]);
    close(fd[1]);
    return 0;
}