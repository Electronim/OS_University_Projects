#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
	if (argc != 2) return 1;
	
	int number = atoi(argv[1]);

	pid_t pid = fork();

	if (pid < 0){
		printf("Fork Failed!\n");
		return 1;
	}	
	else if (pid == 0){
		while (number != 1){
			printf("%d, ", number);
			number = (number & 1 ? 3 * number + 1 : number / 2);
		}

		printf("%d\n", number);
	}
	else{
		wait(NULL);
		
		printf("Child %d finished\n", getpid());
	}
	return 0;
}
