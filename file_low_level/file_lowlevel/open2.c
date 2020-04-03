#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* 해당 파일 안에 데이터를 추가 */
static int append_file(void)
{
    int fd;

    fd = open("Namin_data", O_WRONLY | O_APPEND);
    if (fd == -1) {
        printf("open faile()\n");
        close(fd);
        return (-1);        
    }
    dprintf(fd, "\nThis's Namin\n");

    close(fd);
    return (0);
}

/*파일 데이터가 존재시 0으로 초기화 핫고 다시 작성*/
static int trunc_file(void) {
    int fd;
    fd = open("Namin_data", O_WRONLY | O_CREAT | O_TRUNC, 0422);
    if (fd == -1) {
        printf("open() error\n");
        close(fd);
        return (-1);
    }
    dprintf(fd, "HI Namin\n");

    close(fd);
    return (0);
}

static int wrtie_file(void)
{
    int fd;
    fd = open("Namin_data", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        printf("open() error\n");
        close(fd);
        return (-1);
    }

    dprintf(fd, "hello World%d\n", 0323);
    close(fd);
    return (0);
}

int main(int argc, char** argv) {
    if (wrtie_file()) {
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