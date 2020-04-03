#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

struct person
{
    char name[16];
    int age;
};

static int write_info(struct person* p) {
    int fd;
    ssize_t ret;

    fd = open("memory_mapping_test", O_CREAT | O_TRUNC | O_APPEND | O_WRONLY, 0644);
    if (fd == -1) {
        printf("open() fail\n");
        close(fd);
        return (-1);
    }

    ret = write(fd, p, sizeof(struct person));
    if (ret == -1) {
        printf("write() faile\n");
        close(fd);
        return (-1);
    } else if(ret != sizeof(struct person)) {
        printf("write() faile(partial write)\n");
        close(fd);
        return (-1);
    }

    close(fd);
    return (0);
}

static int dump_info(void) {
    int fd;
    struct stat sb;
    struct person* p;
    int i;

    fd = open("memory_mapping_test", O_RDONLY);
    if (fd == -1) {
        printf("open() faile\n");
        return (-1);
    }

    if (fstat(fd,&sb) == -1) {
        printf("stat() fail\n");
        close(fd);
        return (-1);
    }

    p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        printf("mmap() fail\n");
        close(fd);
        return (-1);
    }
    for (i = 0; i < sb.st_size / sizeof(struct person); i++) {
        printf("name: %s, age: %d\n",p[i].name, p[i].age);
    }

    close(fd);
    return(0);
}

int main(int argc, char** argv) {
    struct person persons[] = {
        {"Namin", 24},
        {"Namin2", 22}
    };
    
    write_info(&persons[0]);
    write_info(&persons[1]);

    dump_info();
    return (0);
}