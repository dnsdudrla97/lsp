#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 파일의 속성을 조회해보고 파일에 대해서 판단하고 출력을 해본다. 파일의 속성을 받아서 여러 정보들을 출력*/
int main(int argc, char** argv) {
    struct stat statbuf;

    /* 해당 파라미터로 파일 이름으로 받아온다 */
    if (argc < 2) {
        printf("usage : %s filename\n", argv[0]);
        return (-1);
    }
    /* stat() 으로 파일 조회*/
    if (stat(argv[1], &statbuf)) {
        printf("stat() fail\n");
        return (-1);
    }

    printf("filename: %s\n", argv[1]);
    printf("size: %ld\n", statbuf.st_size);
    
    /* 파일 타입을 확인*/
    if (S_ISDIR(statbuf.st_mode)) {
        printf("it is directory\n");
    } else if (S_ISREG(statbuf.st_mode)) {
        printf("it is regular file\n");
    } else {
        printf("????\n");
    }

    return (0);
}