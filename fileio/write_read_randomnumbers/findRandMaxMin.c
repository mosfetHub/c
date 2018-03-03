#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	// Take file name as input
	char filename[50];
	printf("Enter the filename: ");
	scanf("%s", filename);

	// File to read from
	FILE *filePointer;
	filePointer = fopen(filename, "r");

	// If file not found, exit
	if (filePointer == NULL) {
		printf("Cannot locate the file. Exit. :(\n");
		exit(0);
	}

	// Read the file anc calculate min and max
	int randNumber;
	int min, max, isFirstNumberRead = 0;
	while ( (fscanf(filePointer, "%d", &randNumber)) != EOF) {
		if (isFirstNumberRead == 0) {
			min = randNumber;
			max = randNumber;
			isFirstNumberRead = 1;
		}
		else {
			if (randNumber < min)
				min = randNumber;
			else if (randNumber > max)
				max = randNumber;
		}
	}

	// Print results
	printf("Minimum random number: %d\nMaximum random number: %d\n", min, max);

	// Close file pointer and exit
	fclose(filePointer);
	return 0;
}