#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE *fp = fopen("testfile", "r+");
	if (fp == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	printf("Length of the sourcefile: ");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	rewind(fp);
	//fseek(fp, 13, SEEK_SET);
	printf("%d\n", length);

	//char *buffer = (char*) malloc(length-12);
	char *buffer = (char*) malloc(length+1);
	char c = fgetc(fp);
	int write_index = 0;
	while (c != EOF) {
		printf("%c", c);
		buffer[write_index++] = c;
		c = fgetc(fp);
	}
	printf("\n");
	buffer[write_index] = '\0';

	buffer = buffer+13;

	for (int i = 0; buffer[i] != '\0'; i++) {
		printf("%c", buffer[i]);
	}
	printf("\n");

	return 0;
}