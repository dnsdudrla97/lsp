#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    FILE* rfp;
    int id, s1, s2, n;
    if ((rfp = fopen("test.dat", "r")) == NULL) {
        perror("fopen: NaminData");
        exit(1);
    }
    while((n=fscanf(rfp, "%d %d %d", &id, &s1, &s2)) != EOF) {
        printf("%d : %d\n", id, (s1 + s2));
    }
    fclose(rfp);

    return (0);
}
