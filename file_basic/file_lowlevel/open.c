#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* 파일 안 데이터 추가*/
static int append_file(void)
{
    int fd;

    /* datafile / O_WRONLY | O_APPEND = 쓰기전용 | 추가(오프셋 파일 끝에 위치)*/
    fd = open("datafile", O_WRONLY | O_APPEND); // return : fd : error -1
    if (fd == -1) {
        printf("open() error\n");
        return (-1);
    }

    dprintf(fd, "\nYOUYOUYOU\n");

    /* open() 하고난뒤 해제*/
    close(fd);
    return (0);
}

static int trunc_file(void)
{
    int fd;
    /* datafile / O_WRONLY | O_CREAT | O_TRUN = 쓰기 전용 | 파일이 존재하지 않으면 정규 파일 생성 | 정규 파일 존재시 사이즈 0으로 변경(다시 생성), 파일 권한 0422 */
    fd = open("datafile", O_WRONLY | O_CREAT | O_TRUNC, 0422); // return : fd : error -1
    if (fd == -1) {
        printf("open() error\n");
        return (-1);
    }

    dprintf(fd, "Namin love!!!\n");

    /* open() 하고난뒤 해제*/
    close(fd);
    return (0);
}

static int write_file(void)
{
    int fd;

    /* 1. path, 2. 권한(R,W,X 하나라도 포함이 되있어야 한다.)*/
    // fd = open("datafile", O_WRONLY | O_CREAT, 0644); // return : fd : error -1

    fd = open("datafile", O_RDWR | O_CREAT, 0666); // return : fd : error -1
    if (fd == -1) {
        printf("open() error\n");
        return (-1);
    }

    dprintf(fd, "hello world!! %d\n", 0323);

    /* open() 하고난뒤 해제*/
    close(fd);
    return (0);
}

int main(int argc, char** argv)
{
    if (write_file()) {
        printf("write_file() error\n");
        return (-1);
    }
    if (trunc_file()) {
        printf("trunc_file() error\n");
        return (-1);
    }
    if (append_file()) {
        printf("append_file() error\n");
        return (-1);
    }

    return (0);
}