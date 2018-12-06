#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <time.h>
#include <fcntl.h> 

#define BUFF_SIZE 20000
#define MOD 11

int main(int argc, char **argv){
	srand(time(NULL));
	
	if (argc < 2) return 1;

    char shm_name[] = "myshm";
    int shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    int *shm_ptr;
    
    if(shm_fd < 0) {
        perror(NULL);
        return errno;
    }

    unsigned int shm_size = (argc - 1) * __getpagesize();
    if (ftruncate(shm_fd, shm_size) == -1) {
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    printf("Starting parrent %d\n", getpid());

	for (int i = 1; i < argc; i++) {
		int number = atoi(argv[i]);	
		
		pid_t pid = fork();
		if (pid < 0){
			printf("Fork Failed!\n");
			exit(1);
		}	
		else if (pid == 0){
            shm_ptr = mmap(0, __getpagesize(), PROT_WRITE, MAP_SHARED, shm_fd, (i - 1) * __getpagesize());
            if (shm_ptr == MAP_FAILED) {
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }

            int counter = 0;

			while (number != 1){
                shm_ptr[counter++] = number;
                number = (number & 1 ? 3 * number + 1 : number / 2);
            }

            shm_ptr[counter++] = number;

			printf("Done Parent %d Me %d\n", getppid(), getpid());
			exit(0);

		}
		else{
            munmap(shm_ptr, __getpagesize());
		}
	}

	for (int i = 1; i < argc; i++) {
        wait(NULL);
    }

    for (int i = 1; i < argc; i++) {
        shm_ptr = mmap(0, __getpagesize(), PROT_READ, MAP_SHARED, shm_fd, (i - 1) * __getpagesize());

        printf("%d: ", atoi(argv[i]));
        for (int i = 0; 1; ++i) {
            if (i > 0 && shm_ptr[i - 1] == 1) break;
            printf("%d ", shm_ptr[i]);
        }
        
        printf("\n");
    }

    shm_unlink(shm_name);
    printf("Done Parent %d Me %d\n", getppid(), getpid());
	return 0;
}
