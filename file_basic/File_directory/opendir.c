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

    /* 현재 디렉터리 오픈*/
    dp = opendir("..");
    if (dp == NULL) {
        printf("opendir() fail\n");
        return (-1);
    }
    /* readdir 디렉터리 엔트리 포인터를 받아온다. */
    /* 모든 파일에 대해서 호출을 해야 되기 때문에 반복을 한다. 실패시 NULL 포인터, 파일이 끝까지 갔을때 NULL 포인터 리턴*/
    while ((entry = readdir(dp))) {
        /*정보를 잘 가져왔다*/
        printf("%s: %s\n", entry->d_name, TYPE2STR(entry->d_type));
    }

    /* 현재 디렉터리 닫기*/
    closedir(dp);
    return (0);
}