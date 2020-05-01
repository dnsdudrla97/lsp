#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// void (*sa_sigaction)(int, siginfo_t *, void *);
void sigusr1_handler(int sig, siginfo_t *info, void *ucontext)
{
    printf("got sigusr1\n");
    /*si_code : 시그널을 일으킨 원인 */
    /*siqueue() 형식으로 왔냐? 다른 방식으로 왔냐?*/
    if (info->si_code & SI_QUEUE)
    {
        /*sig_int : sigqueue()를 통해 보낸 시그널 페이로드*/
        printf("sigqueue() send %d\n", info->si_int);
    }
    else
    {
        printf("kill() send\n");
    }
}

/*sigaction*/
// struct sigaction {
//                void     (*sa_handler)(int);
//                void     (*sa_sigaction)(int, siginfo_t *, void *);
//                sigset_t   sa_mask;
//                int        sa_flags;
//                void     (*sa_restorer)(void);          /*사용되지 않음*/
// };

int main(int argc, char **argv)
{
    /*특정 시그널의 동작*/
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));

    // SA_SIGINFO를 반드시 포함, -> sa_sigaction 호출가능
    act.sa_flags = SA_SIGINFO;

    // void (*sa_sigaction)(int, siginfo_t *, void *);
    act.sa_sigaction = sigusr1_handler;

    /*sigaction(signum, 시그널 처리 액션(시그널 동작), 기존 액션(NULL)*/
    if (sigaction(SIGUSR1, &act, NULL))
    {
        printf("sigaction() fail\n");
        return -1;
    }

    while (1)
    {
        sleep(1);
    }

    return 0;
}