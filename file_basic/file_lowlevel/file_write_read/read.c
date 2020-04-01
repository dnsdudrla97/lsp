#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

/*LowLevel 파일 열어서 strut person 만큼씩 데이터를 읽어서 화면에 출력*/
static int dump_info(void) {
    int fd;
    ssize_t ret;    /* ssize_t */
    struct person p;

    fd = open("person_info", O_RDONLY);
    if (fd == -1) {
        printf("open() faill\n");
        return -1;
    }

    do {
				/* read함수로 구조체 p 에 데이터를 할당*/
        ret = read(fd, &p, sizeof(struct person));
        if (ret == -1) {
            printf("read() faile\n");
            close(fd);
            return (-1);
        }
        else if (ret == 0) {       /*EOF 시 0을 반환*/
            FILE* fp;     /*0 반환시 파일의 끝의 주소값을 알기 위해서 파일 포인터 사용*/
            fp = fdopen(fd, "r"); /*파일 디스크립터 → 파일 포인터 변환*/
            if (fp == NULL) {
                printf("fdopen() fail\n");
                close(fd);
                return (-1);
            }
            printf("file position = %ld\n",ftell(fp)); /* frell() 현재 파일의 오프셋 위치 */
            fclose(fp);
            break;
        }
				/*해당하는 사이즈 개수 만큼 안나왔을때 예외처리*/
        else if (ret != sizeof(struct person)) {
			
            printf("read() faile(partial read)\n");  
            close(fd);
            return (-1);
        }
        printf("name : %s, age : %d\n",p.name, p.age);
    } while(1);

    close(fd);
    return (0);
}

int main(int argc, char** argv) {
    struct person persons[] = {
        { "Namin", 24 },
        { "Woon", 30 },
    };

    write_info(&persons[0]);
    write_info(&persons[1]);

    dump_info();

    return (0);
}