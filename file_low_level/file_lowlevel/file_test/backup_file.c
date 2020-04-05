
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
struct NaminData {
    char name[16];
    int age;
};

static int set_data(struct NaminData* p)
{
    int fd;
    ssize_t ret;
    mode_t mode;

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    // 0400 0200 0040 0004 = 0644


    fd = open("NaminData", O_CREAT | O_WRONLY | O_APPEND, mode);
    if (fd == -1)
    {
        perror("Open NaminData set_data");
        exit(1);
    }

    ret = write(fd, p, sizeof(struct NaminData));
    if (ret == -1) {
        perror("wrtie");
        close(fd);
        exit(1);
    } else if (sizeof(struct NaminData) != ret) {
        perror("write");
        close(fd);
        exit(1);
    }

    close(fd);
    return (0);
}

static int dump_data(void)
{
    int fd;
    ssize_t ret;
    struct NaminData p;

    fd = open("NaminData", O_RDONLY);
    if (fd == -1)
    {
        perror("Open NaminData Dump");
        exit(1);
    }

    do {
        ret = read(fd, &p, sizeof(struct NaminData));
        printf("read :%ld\n",ret);
        if (ret == -1) {
            perror("read");
            close (fd);
            exit(1);
        } else if(sizeof(struct NaminData) != ret) {
            perror("read");
            close(fd);
            exit(1);
        } else if(ret == 0) {
            FILE* fp;
            fp = fdopen(fd, "r");
            if (fp == NULL) {
                perror("fdopen");
                close(fd);
                exit(1);
            }
            printf("file position = %ld\n", ftell(fp));
            fclose(fp);
            break;
        }
        printf("name : %s, age : %d\n",p.name, p.age);
    } while (1);

    close (fd);
    return (0);
}

static int backup(void)
{
    int fd, wfd;
    ssize_t ret;
    struct NaminData t;
    mode_t mode;

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd = open("NaminData", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        close(fd);
        exit(1);
    }

    wfd = open("NaminData2", O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (wfd == -1)
    {
        perror("open");
        close(wfd);
        exit(1);
    }

    
    // while ((ret = read(fd, &t, sizeof(struct NaminData)) > 0 )) {
    //     printf("read : %ld\n", ret);
    //     if (write(wfd, &t, ret) != ret) {
    //         perror("write");
    //         // close(wfd);
    //         // exit(1);    
    //     }
    // }
    ret = read(fd, &t, sizeof(struct NaminData));
    if (ret == -1){
        perror("read:");
        close(fd);
        exit(1);
    } else if (ret != sizeof(struct NaminData)) {
        perror("read:partial");
        close(fd);
        exit(1);
    }
    
    if (write(wfd, &t, ret) != ret)
        perror("write");

    close(fd);
    close(wfd);
    return (0);
}

int main(int argc, char** argv)
{
    struct NaminData p[] = {
        { "Namin", 20 },
        { "Namin2", 30}
    };

    set_data(&p[0]);
    set_data(&p[1]);
    backup();

    dump_data ();
    return (0);
}