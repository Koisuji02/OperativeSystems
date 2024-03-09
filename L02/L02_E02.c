#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int i;
	int n = atoi(argv[1]);
	int t = atoi(argv[2]);
	for (i = 0; i < n; i++) {
		fork();
	}
	sleep(t);
	printf("End!\n");
	printf("Processo: %d\n", getpid());
	return(1);
}
