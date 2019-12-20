#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[]) {
	FILE* fp;
	int mm;
	char nn;
	printf("Read (0) or Write (1)?\n");
	scanf("%c", &nn);
	if(nn == '0') {
		fp = fopen("room0.djm", "r");
		while(mm = fgetc(fp), mm != EOF) {
			printf("%d\n", mm);
		}
	}
	else {
		fp = fopen("room0.djm", "w");
		while((printf("Enter value.\n"), scanf("%d", &mm)), mm != -1) {
			fputc(mm, fp);
		}
	}
	fclose(fp);
	system("PAUSE");
	return 0;
}