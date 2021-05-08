#include <stdlib.h>
#include <unistd.h>

char* cmd1[] = {"ps", "aux", NULL};
char* cmd2[] = {"sort", "-nrk", "3,3", NULL};
char* cmd3[] = {"head", "-5", NULL};

int pid;
int pipe1[2];
int pipe2[2];

void exec1 () {
	dup2(pipe1[1], 1);

	close(pipe1[0]);
	close(pipe1[1]);

	execv("/bin/ps", cmd1);
}

void exec2 () {
	dup2(pipe1[0], 0);
	dup2(pipe2[1], 1);

	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	execv("/bin/sort", cmd2);
}

void exec3 () {
	dup2(pipe2[0], 0);

	close(pipe2[0]);
	close(pipe2[1]);

	execv("/bin/head", cmd3);
}

int main () {
	pipe(pipe1);

	if ((pid = fork()) == 0) {
		exec1();
	}

	pipe(pipe2);

	if ((pid = fork()) == 0) {
		exec2();
	}

	close(pipe1[0]);
	close(pipe1[1]);

	if ((pid = fork()) == 0) {
		exec3();
	}
}
