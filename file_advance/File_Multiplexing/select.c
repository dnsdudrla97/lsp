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


int main(int argc, char** argv)
{
    int fd = -1;
    int wd = -1;
    int wd2 = -1;
    char buf[1024];
    int ret;
    struct inotify_event* event;
    fd_set fds;
    struct timeval timeout;

    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init() : ");
        return (-1);
    }

    wd = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("add_watch() : ");
        goto err;
    }
    
    wd2 = inotify_add_watch(fd, "/tmp", IN_CREATE | IN_DELETE);
    if (wd2 == -1) {
        perror("add_watch() : ");
        goto err;
    }

    while(1) {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        FD_SET(STDIN_FILENO, &fds);
        
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        ret = select(fd > STDIN_FILENO ? fd + 1 : STDIN_FILENO + 1,
                &fds, NULL, NULL, &timeout);
        if (ret == -1) {
            goto err;
        } else if(ret == 0) {
            perror("timeout occur!!");
        } else if (ret > 0) {
            /*inotify event*/
            if (FD_ISSET(fd, &fds)) {

                ret = read(fd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read() :");
                    break;
                }

                event = (struct inotify_event*)&buf[0];
                while (ret > 0) {
                    if (event->mask & IN_CREATE) {
                        printf("file %s is created\n", event->name);
                    }
                    if (event->mask & IN_DELETE) {
                        printf("file %s is deleted\n", event->name);
                    }
                    event = (struct inotify_event*)(char*)event + sizeof(struct inotify_event) + event->len;
                    ret -= (sizeof(struct inotify_event) + event->len);
                }

            } else if (FD_ISSET(STDIN_FILENO, &fds)) {      /*STDIN_FILENO event*/
                memset(buf, 0 ,sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read() :");
                    break;
                }
                printf("user input [%s]\n", buf);
            }
        }        
    }
    close(wd);
    close(wd2);
    close(fd);
    return (0);
err:
    if (fd >= 0) {
        close(fd);
    }
    if (wd >= 0) {
        close(wd);
    }
    if (wd2 >= 0) {
        close(wd2);
    }
    return (-1);
}