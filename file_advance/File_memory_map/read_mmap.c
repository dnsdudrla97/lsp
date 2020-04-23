#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

/*LowLevel 파일 쓰기 구조체를 주로 사용함*/
struct person
{
    char name[16];
    int age;
};

/* 사람 정보를 계속 해서 추가 하는 */
static int write_info(struct person* p) {
    int fd;
    ssize_t ret;    /* ssize_t */


    /*파일이 있는 경우에는 기존에 있던 정보 뒤에 작성, 파일이 없는 경우 파일을 생성*/
    fd = open("person_info", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1) {
        printf("open() faill\n");
        return -1;
    }

    /*사람에 대한 정보를 파일에 저장 : 해당 write 함수 반환값 에러 처리 2개*/
    ret = write(fd, p, sizeof(struct person));
    if (ret == -1) {
        printf("write() faile\n");
        close(fd);
        return (-1);
    } else if (ret != sizeof(struct person)) {
        printf("write() faile(partial write)\n");       /* 일부만 에러 예외처리 Parial write*/
        close(fd);
        return (-1);
    }

    close(fd);
    return (0);
}

/*mmap*/
static int dump_info(void) {
    int fd;
    struct stat sb;
    struct person* p;
    int i;

    fd = open("person_info", O_RDONLY);
    if (fd == -1) {
        printf("open() fail\n");
        return (-1);
    }

    if (fstat(fd, &sb) == -1) {
        printf("stat() fail\n");
        close(fd);
        return (-1);
    }

    /*void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset); */
    /* return 성공시 mapping된 주소 / 실패시 MAP_FAILED */
    p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        printf("mmap() fail\n");
        close(fd);
        return (-1);
    }
    /* 파일 전체에 대해서 하나의 메모리 주소를 매핑을 해와서 실제
     메모리 상에 있는 데이터구조를 조작하는 것처럼, 
     p라는 포인터를 이용해서 파일의 이곳저곳을 탐색할 수 있다. */
    for (i = 0; i < sb.st_size / sizeof(struct person); i++) {
        printf("name: %s, age: %d\n",p[i].name, p[i].age);
    }

    close(fd);
    return (0);
}

int main(int argc, char** argv) {
    struct person persons[] = {
        { "Namin", 24 },
        { "Woon", 30 },
	{ "this", 20 },
	{ "solder", 10 },
	{ "pubal", 34 },
	{ "valas", 12 },
	{ "lawer", 10 },
    };
    /* byte stream 저장 */
    write_info(&persons[0]);
    write_info(&persons[1]);
    write_info(&persons[2]);
    write_info(&persons[3]);
    write_info(&persons[4]);
    write_info(&persons[5]);
    write_info(&persons[6]);

    dump_info();

    return (0);
}
