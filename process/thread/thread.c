#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_TRY 100000000
/*전역변수 : thread들간의 공유되는 자원중 하나*/
int shared = 0;

/*Mutex 모든 therad가 공유를 해야 한다.*/
pthread_mutex_t mutex;

void *child_thread_main(void *arg)
{
    unsigned int i;
    
    /*동작중인 thread*/
    printf("created pthread id %ld\n", pthread_self());

    /*Critical Section 짐입시 lock, 나올시 unlock*/
    pthread_mutex_lock(&mutex);

    for (i = 0; i < MAX_TRY; i++)
    {
        // pthread_mutex_lock(&mutex);
        shared++;
        // pthread_mutex_unlock(&mutex);
    }

    for (i = 0; i < MAX_TRY; i++)
    {
        // pthread_mutex_lock(&mutex);
        shared--;
        // pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{

    int ret;
    pthread_t child_thread[2];
    
    /*Sync*/
    if (pthread_mutex_init(&mutex, NULL)) {
        printf("mutex init fail\n");
        return -1;
    }

    printf("main pthread id %ld\n", pthread_self());

    /*start rountine -> new thread main */
    /*thread 0*/
    ret = pthread_create(&child_thread[0], NULL, child_thread_main, NULL);
    if (ret)
    {
        printf("pthread_create(%d) fail", ret);
        return -1;
    }

    /*start rountine -> new thread main */
    /*thread 1*/
    ret = pthread_create(&child_thread[1], NULL, child_thread_main, NULL);
    if (ret)
    {
        printf("pthread_create(%d) fail", ret);
        return -1;
    }

    printf("trying to join %ld\n", child_thread[0]);
    /*resource 정리 (종료 대기)*/
    if (pthread_join(child_thread[0], NULL))
    {
        printf("pthread_join(%ld) fail\n", child_thread[0]);
    }

    printf("trying to join %ld\n", child_thread[1]);
    /*resource 정리 (종료 대기)*/
    if (pthread_join(child_thread[1], NULL))
    {
        printf("pthread_join(%ld) fail\n", child_thread[1]);
    }

    printf("shared = %d\n", shared);
    printf("Done.\n");
    return 0;
}