#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    FILE* rfp;
    char buf[BUFSIZ];
    int n;

    if((rfp = fopen("linux.txt", "r")) == NULL) {
        perror("fopen: linux.txt");
        exit(1);
    }

    while((n=fread(buf, sizeof(char)*2, 3, rfp)) > 0) {
        buf[6] = '\0';
        printf("n=%d, buf=%s\n", n, buf);
    }

    fclose(rfp);

    return (0);
}