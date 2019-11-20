#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

const char * path = "/dev/leds";

void printBinary(char * name, char toBinary) {
	int i;
	printf("%s :", name);
	for (i = 0; i < 8; i++) {
		printf("%d", !!((toBinary << i) & 0x80));
	}
	printf("\n");
}

void writeLEDs(char buffer) {
	int fileDescritpion = open(path, O_WRONLY, S_IWUSR);

	if (fileDescritpion == -1) {
		printf("open: %s ", strerror(errno));
	}
	int writeSize = write(fileDescritpion, &buffer, sizeof(buffer));

	if (writeSize == -1) {
		printf("write: %s ", strerror(errno));
	}
	close(fileDescritpion);
}

int main(int argc, char *argv[]) {

	int input;
	while (1) {

		printf("Enter led1: ");
		input = getchar();
		while (getchar() != '\n') {
			;
		}

		char led1 = (char) (input - '0');
		printBinary("led1", led1);

		printf("Enter led2: ");
		input = getchar();
		while (getchar() != '\n') {
			;
		}
		char led2 = (char) (input - '0');
		printBinary("led2", led2);

		char led3 = led1 | led2;
		printBinary("led3", led3);

		char led4 = led1 ^ led2;
		printBinary("led4", led4);

#if 0
		led4 = led4 << 1;
		led4 = led4 | led3;
		led4 = led4 << 1;
		led4 = led4 | led2;
		led4 = led4 << 1;
		led4 = led4 | led1;
#else
		led4 = led4 << 3 | led3 << 2 | led2 << 1 | led1;
#endif
		writeLEDs(led4);
		printf("\n");
	}

	return EXIT_SUCCESS;
}

