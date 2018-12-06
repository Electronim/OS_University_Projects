#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define MOD 100

void getCollatz(int number){
	while (number != 1){
		printf("%d, ", number);
		number = (number & 1 ? 3 * number + 1 : number / 2);
	}
	sleep(rand() % MOD);

	printf("%d\n", number);
}

int main(int argc, char **argv){
	srand(time(NULL));
	
	if (argc < 2) return 1;

	for (int i = 1; i < argc; i++) {
		int number = atoi(argv[i]);	
		
		pid_t pid = fork();
		if (pid < 0){
			printf("Fork Failed!\n");
			exit(1);
		}	
		else if (pid == 0){
			getCollatz(number);

			printf("Done Parent %d Me %d\n", getppid(), getpid());
			exit(0);
		}
		else{
			
		}
	}

	for (int i = 1; i < argc; i++) wait(NULL);

	return 0;
}
