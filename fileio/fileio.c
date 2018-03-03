#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("use: ./<prog> <filename>\n");
		exit(0);
	}

	char *filename = (char*) malloc(strlen(argv[1])+1);
	strcpy(filename, argv[1]);
	
	FILE *fp = fopen(filename, "r+");
	if (fp == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	printf("Length of the sourcefile: ");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("%d\n", length);

	printf("\nReading the file...\n");
	char c = fgetc(fp);
	while (c != EOF) {
		printf("%c", c);
		c = fgetc(fp);
	}

	printf("\nWriting at the begining of the file...\n");
	fseek(fp, 0, SEEK_SET);
	char *buffer = (char*) malloc(length+1);
	c = fgetc(fp);
	int i = 0;
	while (c != EOF) {
		buffer[i++] = c;
		c = fgetc(fp);
	}
	
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "Written at the beginning. ");
	fputs(buffer, fp);
	printf("Writing done.\n");

	printf("\nReading the file again...\n");
	fseek(fp, 0, SEEK_SET);
	c = fgetc(fp);
        while (c != EOF) {
                printf("%c", c);
                c = fgetc(fp);
        }


	fclose(fp);
	free(buffer);
	free(filename);
	return 0;
}
