#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv)
{
    pid_t pid;
    int exit_status;
    printf("current process : %d\n", getpid());
    pid = fork();
    if (pid == 0) {
        printf("Child Process : %d\n", getpid());
        if (execl("/bin/ls", "ls", "-al", ".", NULL) == -1) {
            perror("execl fail : ");
            return -1;
        }
    }
    pid = wait(&exit_status);
    if (WIFEXITED(exit_status)) {
        printf("child %d return %d\n", pid, WEXITSTATUS(exit_status));
    } else {
        printf("child %d is not exited\n", pid);
    }
    return 0;
}