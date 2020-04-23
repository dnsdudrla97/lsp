    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/file.h>
    #include <error.h>
    #include <errno.h>
    
    static void print_usage(const char* progname) {
        printf("usage: %s (ex | sh)\n", progname);
        exit(-1);
    }


    /* 여러 개 파일이 동시에 동기화를 시도한다.????*/
    int main(int argc, char** argv) {
        
        int fd;
        int ops;

        if (argc < 2) {
            print_usage(argv[0]);
        }

        if (!strcmp(argv[1], "sh")) {
            ops = LOCK_SH;
        } else if (!strcmp(argv[1], "ex")) {
            ops = LOCK_EX;
        } else {
            print_usage(argv[0]);
        }


        fd = open("lockfile", O_RDWR | O_CREAT, 0644);
        if (fd < 0) {
            printf("open() fail\n");
            return (-1);
        }
        /*check flock*/
        printf("Trying to grab the lock\n");
        /* lock을 잡고 잠시동안 사용자의 입력을 기다리고 사용자가 입력하면 lock을 해제 하고*/
        /* NB 누군가 락을 획득하고 있고 내가 락을 획득하지 못하는 상황일 때 LOCK_NB를 통해서 에러를 처리*/
        if (flock(fd, ops | LOCK_NB) != 0) {
            printf("flock(ops %d), errno=%d(%s)\n",ops, errno, strerror(errno));
            goto out;
        }
        /*check flock*/
        printf("get the lock\n");

        /*getc stdin -> 표준 입출력으로 아무키나 */
        getc(stdin);
        if (flock(fd, LOCK_UN) != 0) {
            printf("flock(UNLOCK) \n");
            goto out;
        }
        close(fd);
        return (0);

    out:
        close(fd);
        return (-1);

    }
