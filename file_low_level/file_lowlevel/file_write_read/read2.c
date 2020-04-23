#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct person
{
    char name[16];
    int age;
};

static int write_info(struct person* p) {
    int fd;
    ssize_t ret;

    fd = open("Namin_info", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1) {
        printf("open fail()\n");
        close(fd);
        return (-1);
    }

    ret = write(fd, p, sizeof(struct person));
    if (ret == -1) {
        printf("write() fail\n");
        close(fd);
        return (-1);
	/*count보다 작을 수 있다. (return 값이 count와 동일한지 체크)*/
    } else if (ret != sizeof(struct person)) {
        printf("write() faile partial write\n");
        close(fd);
        return (-1);
    }

    close(fd);
    return (0);
}

static int dump_info(void) {
    int fd;
    ssize_t ret;
    struct person p;

    fd = open("Namin_info", O_RDONLY);
    if (fd == -1) {
        printf("open() fail\n");
        close(fd);
        return (-1);
    }

    do {
        ret = read(fd, &p, sizeof(struct person));
        if (ret == -1) {
            printf("read() faile\n");
            close(fd);
            return (-1);
        }
        else if(ret == 0) {     /*return = 0 -> 파일의 끝을 만났을때*/
			
    	    FILE* fp;		/*ftell() API 사용을 위해서 선언*/
            fp = fdopen(fd, "r");           /*fdopen(fd, "r") -> fd -> fp read*/
            if (fp == NULL) {
                printf("fdopen() fail\n");
                close(fd);
                return (-1);
            }
            printf("file position = %ld\n", ftell(fp));
            fclose(fp);
            break;
        }
        else if (ret != sizeof(struct person)) {	/*return count 가 다를때*/
            printf("read() fail(partial read)\n");
            close(fd);
            return(-1);
        }
        printf("name : %s, age : %d\n", p.name, p.age);
    } while(1);

    close(fd);
    return (-1);
}
 
int main(int argc, char** argv) {
    struct person persons[] = {
        { "NaminNamin", 24 },
        { "WoonWOon", 23},
    };

    write_info(&persons[0]);
    write_info(&persons[1]);

    dump_info();

    return (0);
}
    
