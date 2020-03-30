#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct - 파일 읽고 쓰기
struct person
{
	char name[20];
	int age;
};

// fwrite 를 사용해서 데이터를 파일에 쓰고 fread를 통해서 파일을 읽어 화면에 출려갸
// struct를 이용해서 파일을 쓰고 읽을 때 유용하다.

// 현재 파일이 없어, 생성, 파일 데이터 쓰기
static int write_to_file(void)
{
	FILE *fp = NULL;

	struct person kim = {
		.name = "kim",
		.age = 20
	};

	struct person lee = {
		.name = "lee",
		.age = 23
	};

	/* 예외 처리 */
	if (!(fp = fopen("persons", "w"))) {
		return (-1);	// NULL 포인터 -1
	}

	/* 파일 저장 1. 포인터, 2. 사이즈, 3. 개수, 4. 파일 포인터*/
	if (fwrite(&kim, sizeof(struct person), 1, fp) != 1) {
		goto err;
	}

	/* 아이템 개수를 반환 = 1개 -> 아닐시 에러처리 */
	if (fwrite(&lee, sizeof(struct person), 1, fp) != 1) {
		goto err;
	}
	
	fclose(fp);
	return (0);

/* 예외처리 - fwrite() 반환값 이 1이 아닌 상태 */
err:
	if (fp) fclose(fp); 
	return (-1);		
}

/* 파일 을 읽어서 실행 */
static int read_from_file(void)
{
	FILE* fp;
	int i;
	struct person persons[2];

	/* 예외 처리 */
	if (!(fp = fopen("persons", "r"))) {
		return (-1);	// NULL 포인터 -1
	}

	if ((fread(persons, sizeof(struct person), 2, fp)) != 2) {
		fclose(fp);
		return -1;
	}

	fclose(fp);
	for (i = 0; i < 2; i++) {
		printf("name: %s, age: %d\n",
		 persons[i].name,
		 persons[i].age);
	}
	return (0);
}


int main(int argc, char** argv)
{
	if (write_to_file()) {
		printf("write fail\n");
		return (-1);
	}

	if (read_from_file()) {
		printf("read fail\n");
		return (-1);
	}
	return (0);
}
