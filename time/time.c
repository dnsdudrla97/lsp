#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#if 0
tm 구조체
 struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };
#endif

/*현재 시간 가져와 -> 화면에 출력 (년, 월, 일, ...) 타이머 동작과정도*/
static void print_cur_time(void)
{
    /*time_t 타입의 변수 생성 : now*/
    time_t now;
    /*시간 정보를 다루기 위해 구조체 변수 생성 : now_tm*/
    struct tm *now_tm;

    /*time(tloc) : tloc : 현재 시간값을 저장할 버퍼*/
    now = time(NULL);
    if (now == -1)
        perror("time : ");
    
    printf("current : %ld ", now);

    /*시간 정보를 다루기*/
    /*localtime : timezone으로 적용된 시간된 시간*/
    now_tm = localtime(&now);
    /*반환값 : 실패시 NULL*/
    if (!now_tm)
    {
        printf("localtime() fail\n");
        return;
    }
    printf("%d/%d/%d %d-%d-%d\n",
           now_tm->tm_year + 1900,
           now_tm->tm_mon + 1,
           now_tm->tm_mday,
           now_tm->tm_hour,
           now_tm->tm_min,
           now_tm->tm_sec);
}

static void sigalarm_handler(int signum)
{
    printf("got SIGALARM\n");
    print_cur_time();
}

int main(int argc, char **argv)
{

    struct itimerval ival;

    /*SIGALARM 처리 등록*/
    signal(SIGALRM, sigalarm_handler);

    /*현재 시간*/
    print_cur_time();

    /*5초 동안 blocking 이 아니라 호출된후 -> 5초뒤 SIGALARM을 보낸다.*/
    alarm(5);

    /*alarm 기다리기*/
    sleep(5);

    /*초기 설정*/
    /*it_value -> 다음 만료 까지의 시간 5초*/
    ival.it_value.tv_sec = 5;
    ival.it_value.tv_usec = 0;

    /*it_interval -> 주기적 타이머 간격 1초*/
    ival.it_interval.tv_sec = 1;
    ival.it_interval.tv_usec = 0;

    /*setitimer -> ITIMER_REAR(뒤 에서부터 카운터 다운 5 4 321), ival(새로설정한 값) -> 기간 만료시 SIGALARM 호출*/
    if (setitimer(ITIMER_REAL, &ival, NULL) != 0)
    {
        printf("setitmer() error\n");
        return -1;
    }

    while (1)
    {
        sleep(1);
    }
    return 0;
}
