#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define TYPE2STR(X) \
    ((X) == DT_BLK  ? "block device" :\
    (X) == DT_CHR   ? "char device" :\
    (X) == DT_DIR   ? "directory" :\
    (X) == DT_FIFO  ? "fifo" :\
    (X) == DT_LNK   ? "symlink" :\
    (X) == DT_REG   ? "regular file" :\
    (X) == DT_SOCK  ? "socket" :\
    "unknown")


int main(int argc, char** argv) {
    DIR* dp;
    struct dirent* entry;

    dp = opendir("/");
    if (dp == NULL) {
        printf("opendir() fail\n");
        return (-1);
    }

    while ((entry = readdir(dp))) {
        printf("%s: %s\n", entry->d_name, TYPE2STR(entry->d_type));
    }

    closedir(dp);
    return (0);
}