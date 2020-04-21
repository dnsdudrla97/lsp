#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/inotify.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if 0
    struct inotify_event {
               int      wd;       /* Watch descriptor */
               uint32_t mask;     /* Mask describing event */
               uint32_t cookie;   /* Unique cookie associating related
                                     events (for rename(2)) */
               uint32_t len;      /* Size of name field */
               char     name[];   /* Optional null-terminated name */
           };
#endif

/* 디렉토리 두개를 지정을 해서 각 디렉토리 안에 파일이 생성되고 삭제될 때 생기는 이벤트를 캐치해서 화면에 출력해보자 ㅇㅇ */
int main(int argc, char **argv)
{

    int fd = -1;
    int wd = -1;
    int wd2 = -1;
    int wd3 = -1;
    char buf[1024];
    int ret;
    struct inotify_event *event;

    fd_set fds;
    struct timeval timeout;

    fd = inotify_init();
    if (fd == -1)
    {
        printf("inotify_init() fail\n");
        return (-1);
    }

    wd = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    if (wd == -1)
    {
        printf("add_watch() fail\n");
        goto err;
    }
    wd2 = inotify_add_watch(fd, "/home/younsle/work/inflearn-lsp/file_basic", IN_CREATE | IN_DELETE);
    if (wd2 == -1)
    {
        printf("add_watch() fail\n");
        goto err;
    }
    wd3 = inotify_add_watch(fd, "/var/log", IN_CREATE | IN_DELETE);
    if (wd3 == -1)
    {
        printf("add_watch() fail\n");
        goto err;
    }

    while (1)
    {
        /*select를 호출하기전에 반드시 fds 설정을 해줘야 한다.*/
        FD_ZERO(&fds);                         /*초기화*/
        FD_SET(fd, &fds);                    
        FD_SET(STDIN_FILENO, &fds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        ret = select(fd > STDIN_FILENO ? fd + 1 : STDIN_FILENO + 1,
                     &fds, NULL, NULL, &timeout);

        /*select return 처리 -1 : error 0 : timeout*/
        if (ret == -1)
        {
            goto err;
        }
        else if (ret == 0)
        {
            printf("select() timeout occur!!\n");
        }
        else if (ret > 0)
        {
            /*하나 이상의 이벤트 발생 fd, STDIN_FILENO*/
            if (FD_ISSET(fd, &fds))
            {
                /*struct inotify_event */
                ret = read(fd, buf, sizeof(buf));
                if (ret == -1)
                {
                    printf("read() fail\n");
                    break;
                }
                /*처음 버퍼에 시작점을 이벤트 포인터로 가져온다.*/
                /*이벤트 포인터 -> 다음위치로 변경 (루프 필요)*/
                event = (struct inotify_event *)&buf[0];
                while (ret > 0)
                {
                    if (event->mask & IN_CREATE)
                    {
                        printf("file %s is created\n", event->name);
                    }

                    if (event->mask & IN_DELETE)
                    {
                        printf("file %s is deleted\n", event->name);
                    }
                    /*byte 단위을 기준으로 이벤트 가 현재 위치에서 구조체 사이즈 + len 만큼 더한값이 그 다음 위치*/
                    event = (struct inotify_event *)(char *)event + sizeof(struct inotify_event) + event->len;
                    ret -= (sizeof(struct inotify_event) + event->len);
                }
            }
            else if (FD_ISSET(STDIN_FILENO, &fds))
            {
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                if (ret == -1)
                {
                    printf("read() fail\n");
                    break;
                }
                printf("user input [%s]\n", buf);
            }
        }
    }
    close(wd);
    close(wd2);
    close(wd3);
    close(fd);

    return (0);
err:
    if (fd >= 0)
    {
        close(fd);
    }
    if (wd >= 0)
    {
        close(wd);
    }
    if (wd2 >= 0)
    {
        close(wd2);
    }
    if (wd3 >= 0)
    {
        close(wd3);
    }

    return (-1);
}