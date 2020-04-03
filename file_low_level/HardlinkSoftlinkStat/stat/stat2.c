#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
    struct stat statbuf;

    if (argc < 2) {
        printf("Usage : %s filename\n", argv[0]);
        return (-1);
    }

    if (stat(argv[1], &statbuf)) {
        printf("stat() fail\n");
        return (-1);
    }

    printf("filename %s\n", argv[1]);
    printf("size: %ld\n", statbuf.st_size);

    if (S_ISDIR(statbuf.st_mode)) {
        printf("it is directory\n");
    }
    else if (S_ISREG(statbuf.st_mode)) {
        printf("it is Regular file\n");
    }
    else {
        printf("Nothing\n");
    }
    return (0);
}