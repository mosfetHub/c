#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
	// Initialize random number generator
	srand((unsigned) time(NULL));
	int randomNumber;

	// File to write to
	FILE *filePointer;
	filePointer = fopen("rnums.txt", "w+");

	// Generate random number and write to the file
	int i;
	for (i = 0; i < 500; i++) {
		randomNumber = rand();
		fprintf(filePointer, "%d\n", randomNumber);
	}

	// Close file pointer and exit
	fclose(filePointer);
	return 0;
}
