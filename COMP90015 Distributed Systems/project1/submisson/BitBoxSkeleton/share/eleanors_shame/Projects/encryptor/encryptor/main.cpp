#include <cstdio>
#include <cstdlib>
#include <windows.h>

int main(int argc, char* argv[]) {
	char buffer[500];
	ZeroMemory(&buffer, sizeof(buffer));
	printf("Opening file...\n");
	FILE* fp = fopen("C:\\Users\\Elias\\Documents\\encrypt.txt", "r");
	if(fp == NULL) {
		printf("Opening file for reading failed.\n");
		perror("Error: ");
		system("PAUSE");
		return 1;
	}
	printf("File opened.\nReading...\n");
	fgets(buffer, 500, fp);
	fclose(fp);
	printf("File contents read.\nOpening file...\n");
	FILE* ffp = fopen("C:\\Users\\Elias\\Documents\\encrypt.txt", "w");
	if(ffp == NULL) {
		printf("Opening file for writing failed.\n");
		perror("Error: ");
		system("PAUSE");
		return 2;
	}
	printf("File opened for writing.\nEncrypting...\n");
	for(int i = 0; i < 500; i++) {
		if(buffer[i] != '\0') {
			printf("Character read:    %c\nCharacter written: %c\n", buffer[i], (char)~(int)buffer[i]);
			fputc((int)~buffer[i], ffp);
		}
		else {
			printf("Null reached.\nBreaking...\n");
			break;
		}
	}
	printf("Success!\n");
	system("PAUSE");
	return 0;
}