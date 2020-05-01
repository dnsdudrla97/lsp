#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/*
 * argv[0] : sigaction_send
 * argv[1] : pid
 * argv[2] : payload
 */

// union sigval {
// 	int sival_int
// 	void *sival_ptr;
// };
int main(int argc, char **argv)
{
    pid_t pid;
    int payload;
    union sigval value;     /*payload*/

    if (argc != 3)
    {
        printf("usage : %s PID PAYLOAD\n", argv[0]);
        return -1;
    }

    pid = atoi(argv[1]);
    value.sival_int = atoi(argv[2]);

    /*sigqueue*/
    /*페이로드와 함께 시그널 전송*/
    sigqueue(pid, SIGUSR1, value);
    return 0;
}
