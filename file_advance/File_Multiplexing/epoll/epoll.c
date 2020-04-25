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

#include <sys/epoll.h>
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

    int epfd = -1;
    struct epoll_event eevent;

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

    /*epoll - Create 1*/
    epfd = epoll_create1(0);
    if (epfd == -1)
    {
        printf("epoll_create1() fail\n");
        goto err;
    }
    /*epoll - control 2*/
    memset(&eevent, 0, sizeof(eevent));
    /*data : epoll wati 에서 그대로 값을 넣는다.*/
    /*
    epoll_ctl 설정전에 event 감시 이벤트를 명시해 주기위해 값을 설정한다.
    events = 이벤트 마스크, data.__ = 감시 대상 fd
    */
    eevent.events = EPOLLIN;
    eevent.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &eevent) == -1)
    {
        printf("epoll_ctl() fail\n");
        goto err;
    }

    /*epoll - control 2*/
    memset(&eevent, 0, sizeof(eevent));
    /*data : epoll wati 에서 그대로 값을 넣는다.*/
    eevent.events = EPOLLIN;
    eevent.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &eevent) == -1)
    {
        printf("epoll_ctl() fail\n");
        goto err;
    }

    while (1)
    {
        /*epoll_wait*/  
        /*timeout : 5초*/
        memset(&eevent, 0, sizeof(eevent));
        /*epoll_wait(epoll instance, 이벤트를 담을 구조체 버퍼, 이벤트 변수의 최대 엔트리 개수, blockingtimeout*/
        ret = epoll_wait(epfd, &eevent, 1, 5000);

        if (ret == -1)
        {
            goto err;
        }
        else if (ret == 0)
        {
            printf("epoll_wait() timeout occur!!\n");
        }
        else if (ret > 0)
        {
            /*이벤트 구조체 멤버 data.fd와 값을 비교하여 일치하는지 체크*/
            if (eevent.data.fd == fd)
            {

                ret = read(fd, buf, sizeof(buf));
                if (ret == -1)
                {
                    printf("read() fail\n");
                    break;
                }

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
            else if (eevent.data.fd == STDIN_FILENO)
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
    close(epfd);
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
    if (epfd >= 0)
    {
        close(epfd);
    }

    return (-1);
}