#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>

int main(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		FILE *f;
		int c;

		f = fopen(argv[i], "r");
		if (!f) {
			perror(argv[i]);
			exit(EXIT_FAILURE);
		}
		while ((c = fgetc(f)) != EOF) {
			if (c == '\t') {
				errno = 0;
				if (fputs("\\t", stdout) == EOF && errno != 0)
					exit(EXIT_FAILURE);
			} else if (c == '\n') {
				errno = 0;
				if (fputs("$\n", stdout) == EOF && errno != 0)
					exit(EXIT_FAILURE);
			} else {
				if (putchar(c) < 0) exit(EXIT_FAILURE);
			}
		}
		fclose(f);
	}
	exit(EXIT_SUCCESS);
}
