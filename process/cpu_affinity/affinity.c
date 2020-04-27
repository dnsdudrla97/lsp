#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXCPU 4
static void Print_cpuset(cpu_set_t *mask)
{
    int i;

    for (i = 0; i < MAXCPU; i++)
    {
        /*CPU_ISSET을 통해서 mask가 맞는지 체크*/
        if (CPU_ISSET(i, mask))
        {
            printf("CPU%d ", i);
        }
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    int cur_cpu;
    cpu_set_t mask;

    /*어느 CPU에서 동작을 하는지 가져온다.*/
    if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask))
    {
        perror("sched_getaffinity() : \n");
        return -1;
    }
    Print_cpuset(&mask);

    /*어느 CPU에서 동작하는지 체크*/
    cur_cpu = sched_getcpu();
    printf("run on CPU : %d\n", cur_cpu);

    /*C프로세서가 동작할 CPU를 변경한다.*/
    
    CPU_ZERO(&mask);        /*CPU_ZERO mask 초기화*/
    CPU_SET(cur_cpu ^ 1, &mask);        /*sched_getcpu()받아온 CPU 번호를 비트 연산하여 mask에 추가*/
    /*추가된 mask값을 바탕으로 setaffinity 수행*/
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask))
    {
        perror("sched_setaffinity()\n");
        return -1;
    }

    sleep(2);
    /*어느 CPU에서 동작하는지 체크*/
    cur_cpu = sched_getcpu();
    printf("now, run on CPU : %d\n", cur_cpu);

    return 0;
}