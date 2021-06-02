#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i;
	FILE *f;
	int c;
	int prev;
	int cnt;

	for (i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");
		if (!f) {
			perror(argv[i]);
			exit(EXIT_FAILURE);
		}

		prev = '\0';
		cnt = 0;
		while ((c = fgetc(f)) != EOF) {
			if (c == '\n') cnt++;
			prev = c;
		}

		/* if end of line has not linefeed */
		if (prev != '\0' && prev != '\n') cnt++;

		printf("%d\n", cnt);
		fclose(f);
	}
	exit(EXIT_SUCCESS);
}
