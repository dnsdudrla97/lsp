#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int cnt;
    /*Return result value*/
    cnt = unlink("tmp.aaa");
    if (cnt == -1)
    {
        perror("Unlink tmp.aaa");
        exit(1);
    }
    printf("Unlink tmp.aaa success !!!\n");
    return(0);
}