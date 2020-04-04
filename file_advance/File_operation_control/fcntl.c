    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/file.h>
    #include <fcntl.h>
    #include <error.h>
    #include <errno.h>

/*read() api를 통해서 값을 읽어오는데 읽어올 데이터 가없을 경우 해당 operator 모드가 어떻게 동작하는지*/
int main(int argc, char** argv) {
    /*fd open 될시 기본적으로 실행되는 fd 0, 1, 2
    STDIN_FILENO
    STDOUT_FILENO
    STDERR_FILENO
    */
    int ret;
    int flag;
    int fd;
    char buf[128];

    fd = STDIN_FILENO;

    printf("Trying to read...\n");
    memset(buf, 0, sizeof(buf));
    /*STDIN_FILENO -> Blocking mode 동작 방식
    STDIN -> 프로그램 종료 시 전부 종료된다.*/
    ret = read(fd, buf, sizeof(buf));
    printf("read() [%s] - %d bytes\n", buf, ret);

    /*NON-BLOCKING MODE 전환*/
    /*현재 fd 상태 읽어 오기*/
    flag = fcntl(fd, F_GETFL);
    if (flag == -1) {
        printf("fcntl(F_GETFL) fail\n");
        return (-1);
    }
    /*설정*/
    ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    if (ret == -1) {
        printf("fcntl(F_SETEL) fail\n");
        return (-1);
    }

    printf("Trying to read...\n");
    memset(buf, 0, sizeof(buf));
    ret = read(fd, buf, sizeof(buf));
    printf("read() [%s] - %d bytes\n", buf, ret);
    if (ret == -1) {
        printf("errno=%d, %s\n",errno, strerror(errno));
    }
    /*read API 부분에서 읽을 데이터가 없으면 에러 결과*/
    return (0);
}