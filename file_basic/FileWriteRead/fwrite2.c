#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Namin Data */
typedef struct Data {
    char name[20];
    char address[20];
    int age;
} _T_Data;

/* -- write -- */
static int write_to() {
    FILE* fp = NULL;
    
    _T_Data namin = { 
        .name = "Namin", 
        .address = "Naju", 
        .age = 24
    };

    if (!(fp = fopen("NaminData", "w")))
        return (-1);

    if (fwrite(&namin, sizeof(_T_Data), 1, fp) != 1) {
        fclose(fp);
        return (-1);
    }

    fclose(fp);
    return (0);
}

static int read_from() {
    FILE* fp;
    _T_Data dataBuf[1];

    if (!(fp = fopen("NaminData", "r")))
        return -1;
    
    if (fread(dataBuf, sizeof(_T_Data), 1, fp) != 1) {
        free(fp);
        return(-1);
    }
    fclose(fp);

    printf("name: %s\naddress: %s\nage: %d\n",dataBuf[0].name, dataBuf[0].address, dataBuf[0].age);

    return(0);

}

int main(int argc, char** argv) {
    if (write_to()) {
        printf("write error \n");
        return (-1);
    }
    if (read_from()) {
        printf("read error \n");
        return (-1);
    }
    return (0);
}