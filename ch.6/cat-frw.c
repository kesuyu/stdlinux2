#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

int main(int argc, char *argv[])
{
	int i;
	FILE *f;
	unsigned char buf[BUFFER_SIZE];
	size_t sz;

	for (i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");
		if (!f) goto err;

		do {
			sz = fread(buf, 1, BUFFER_SIZE, f);
			if (sz < BUFFER_SIZE && ferror(f)) goto err;
			if (fwrite(buf, 1, sz, stdout) < sz) goto err;
		} while(!feof(f));
		fclose(f);
	}
	exit(EXIT_SUCCESS);
err:
	perror(argv[i]);
	exit(EXIT_FAILURE);
}
