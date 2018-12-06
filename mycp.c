#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int buffSize, nbytes;
char *buff;

int main(int argc, char **argv){
	int descrInput = open(argv[1], O_RDONLY);
	if (descrInput < 0){
		char output[] = "No such file to open!\n";		
		write(1, output, sizeof(output));

		return 0;	
	}

	struct stat sb;
	stat(argv[1], &sb);

	buffSize = sb.st_size;
	buff = (char *) malloc(buffSize + 1);

	int descrOutput = open(argv[2], O_WRONLY | O_CREAT);
	fchmod(descrOutput, S_IRWXU);
	
	while (nbytes = read(descrInput, buff, buffSize)){
		int written = 0;
		while ((written = write(descrOutput, buff, nbytes)) < nbytes){
			buff += written;			
			nbytes -= written;
		}
		
	}
	
	free(buff);
	close(descrInput); close(descrOutput);
	return 0;
}
