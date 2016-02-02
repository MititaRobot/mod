#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "playSound.h"

void playSound(char *filename, int options)
{
	pid_t pid;
	char *command[5];
	command[0] = "aplay";
	command[1] = "-D";
	command[2] = "plughw:1,0";
	command[3] = filename;
	command[4] = NULL;

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	} else if (pid == 0) { // child
		execvp(command[0], command);
		exit(1);
	} else {						// parent
		waitpid(pid, NULL, options);
	}
}
