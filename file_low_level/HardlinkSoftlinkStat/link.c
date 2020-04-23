#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    #define TARGET_FILE "hello_world" /*Hello_world file -> SL, HL*/

    unlink("hello_by_link");
    unlink("hello_by_symlink");

    if (link(TARGET_FILE, "hello_by_link")) {
        printf("link() fail\n");
        return (-1);
    }
    if (symlink(TARGET_FILE, "hello_by_symlink")) {
        printf("symlink() fail\n");
        return (-1);
    }
    
    return (0);
}

