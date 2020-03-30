#include <stdio.h>
#include <string.h>

int write_to_file(void) {
	FILE* fp;
	fp = fopen("testData", "w");
	if (fp == NULL) {
		perror("fopen error\n");
		return (-1);
	}
	fputs("hello, Wolrd...", fp);
	fclose(fp);
	return (0);
}

int read_from_file(void)
{
	FILE* fp;
	char buf[1024];

	fp = fopen("testData", "r");
	if (fp == NULL) {
		perror("fread error\n");
		return (-1);
	}

	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	return (0);
}

int main(int argc, char** argv) {
	if (write_to_file()) {
		perror("write to file\n");
		return (-1);
	}
	if (read_from_file()) {
		perror("read to file\n");
		return (-1);
	}

	return (0);
}

