#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>

#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
	{"lf",   no_argument, NULL, 'l'},
	{"tab",  no_argument, NULL, 't'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};

struct config {
	int showLf;
	int showTab;
};

static struct config mycfg;

static void do_cat(FILE *f);

int main(int argc, char *argv[])
{
	int opt;
	int i;

	mycfg.showLf = 0;
	mycfg.showTab = 0;

	while ((opt = getopt_long(argc, argv, "lth", longopts, NULL)) != -1) {
		switch (opt) {
		case 'l':
			mycfg.showLf = 1;
			break;
		case 't':
			mycfg.showTab = 1;
			break;
		case 'h':
			fprintf(stdout, "Usage: %s [-l] [-t] [FILE ...]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case '?':
		default:
			fprintf(stderr, "Usage: %s [-l] [-t] [FILE ...]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	for (i = optind; i < argc; i++) {
		FILE *f;
		f = fopen(argv[i], "r");
		if (!f) {
			perror(argv[i]);
			exit(EXIT_FAILURE);
		}
		do_cat(f);
		fclose(f);
	}
	exit(EXIT_SUCCESS);
}

static void do_cat(FILE *f)
{
	int c;
	while ((c = fgetc(f)) != EOF) {
		if (mycfg.showTab && c == '\t') {
			if (fputs("\\t", stdout) == EOF)
				exit(EXIT_FAILURE);
		} else if (mycfg.showLf && c == '\n') {
			if (fputs("$\n", stdout) == EOF)
				exit(EXIT_FAILURE);
		} else {
			if (putchar(c) < 0)
				exit(EXIT_FAILURE);
		}
	}
}
