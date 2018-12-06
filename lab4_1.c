#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	
	pid_t pid = fork();

	if (pid < 0){ 
		printf("Fork Failed!\n");
		return 1;
	}	
	else if (pid == 0){ // child
		execlp("/bin/ls", "ls", NULL);
	}
	else{ // parent
		printf("My PID = %d ", getppid());
		printf(", Child PID = %d\n", getpid());

		wait(NULL);		
		printf("Child %d finished\n", getpid());
	}
	return 0;
}
