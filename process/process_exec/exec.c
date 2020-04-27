#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void test_func(void)
{
	printf("this is a test function(pid : %d)\n", getpid());
}

int main(int argc, char** argv)
{
	pid_t pid;
	int exit_status;
	
	printf("origin process pid : %d\n", getpid());
	/*fork 호출 (그대로 복제)*/
	pid = fork();

	/*process , new process -> parent, child*/
	/*chold process*/
	if (pid == 0) {
		/*child process*/
		printf("child process pid : %d\n", getpid());
		
		/*아규멘트를 리스트 형태로 path, arg,... NULL(가변인자의 끝을 알려준다.)*/
		/*절대경로로 해야 안전하게 동작*/
		/*ls -al -alt...*/
		/*argv 첫번째 자기자신 이기 때문에 ls 를 집어 넣는다.*/
		/*echo $1 -> 쉴에서 리턴값 확인*/
		if (execl("/bin/ls", "ls", "-al", NULL) == -1) {
			printf("execl() fail\n");
			return -1;
		}
		// if (execl("/bin/ps", "ps", "-t", NULL) == -1) {
		// 	printf("execl() fail\n");
		// 	return -1;
		// }
		/*성공했다면 더이상 이 소스코드내에서 실행 하지 않는다. -> 새로운 코드로 이동*/

	} else if (pid > 0) {
		/*parent process (child pid)*/
		printf("parent process pid : %d, child pid : %d\n", getpid(), pid);
	}
	test_func();
	printf("HHIHIHIHIHIHIHIHIHIHI\n\n");

	/*자식 프로세스를 기다린다.(종료될때까지)*/
	pid = wait(&exit_status);		/*exit_status 에 종료 상태값이 들어온다.*/
	if (WIFEXITED(exit_status)) {
		printf("child %d returns %d\n", pid, WEXITSTATUS(exit_status));		/*종료된 child pid,*/
	} else {
		printf("child %d is not exited\n", pid);
	}
	
	return 0;
}
