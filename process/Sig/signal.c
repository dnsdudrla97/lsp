#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
/*시그널 보내기 (bash - 시그널 보내기)
어플리케이션 (시그널을 받으면 어떻게 동작할지)
*/
/*붙잡고 -> 이 함수 실행*/
void sigterm_handler(int signum)
{
    printf("got sigterm..\n");
    // exit(-1); 
}

int main(int argc, char** argv)
{
    /*시그널 블록*/
    sigset_t set;
    /*sigset 초기화*/
    sigemptyset(&set);
    /*set 변수에 SIGINT추가*/
    sigaddset(&set, SIGINT);

    /*set 설정을 블록킹 하겠다. (SIGINT일 경우 blocking) (SIGINT는 동작을 하지 않는다.)*/
    sigprocmask(SIG_SETMASK, &set, NULL);

    /*시그널 붙잡기 SIGTERM 등록*/
    signal(SIGTERM, sigterm_handler);

    /*테스트*/
    printf("start of while(1)\n");
    while(1) {
        sleep(1);

    }
    return 0;
}