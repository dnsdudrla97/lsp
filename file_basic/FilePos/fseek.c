#include <stdio.h>
#include <string.h>



// 파일 생성 후 내용 추가
static int create_file(void)
{	
	FILE *fp;
	if (!(fp = fopen("datafile", "w"))) {
		printf("fopen() fail\n");
		return (-1);
	}
	
	// 파일 포지션 값 읽어오기
	printf("after fopen(). offset=%ld\n",ftell(fp));
	fputs("hello World\n", fp);
	fputs("hello World!\n", fp);
	fputs("hello World!!\n", fp);
	printf("middle fopen(). offset=%ld\n",ftell(fp));
	fputs("hello World!!!\n", fp);
	fputs("hello World!!!!\n", fp);
	printf("before fclose(). offset=%ld\n", ftell(fp));
	printf("---------------------------\n");

	fclose(fp);
	return (0);
}

static int read_file(void)
{
	FILE *fp;
	char buf[1024];	// fgets() 

	// 읽고 쓰기 권한
	if (!(fp = fopen("datafile", "r+"))) {
		printf("foepn() fail\n");
		return (-1);
	}

	printf("after fopen(). offset=%ld\n", ftell(fp));
	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	for (int i = 0; i < sizeof(buf); i++) {
		printf("%c",buf[i]);
	}
	printf("after fgets(). offset=%ld\n", ftell(fp));
	
	fseek(fp, 0, SEEK_END);	// 파일의 끝을 기준으로 옵셋이 0
	printf("after fseek(). offset=%ld\n", ftell(fp));
	fputs("final\n", fp);
	printf("before fclose(). offset=%ld\n", ftell(fp));
	printf("-------------------------\n");

	
	fclose(fp);
	return (0);
}


int main(int argc, char** argv)
{
	create_file();
	
	read_file();

	return (0);
}
