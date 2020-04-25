#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
void test_func(void)
{
	printf("this is a test function(pid : %d)\n", getpid());
}

int main(int argc, char** argv)
{
	pid_t pid;
	
	printf("origin process pid : %d\n", getpid());
	/*fork 호출 (그대로 복제)*/
	pid = fork();

	/*process , new process -> parent, child*/
	if (pid == 0) {
		/*child process*/
		printf("child process pid : %d\n", getpid());
	} else if (pid > 0) {
		/*parent process (child pid)*/
		printf("parent process pid : %d, child pid : %d\n", getpid(), pid);
	}
	test_func();
	
	return 0;
}
