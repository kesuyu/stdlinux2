#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
	{"ignore-case",  no_argument, NULL, 'i'},
	{"invert-match", no_argument, NULL, 'v'},
	{"help",         no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};

struct config {
	int is_v;
};

static struct config mycfg;

static void do_slice(regex_t *pat, FILE *f);

int main(int argc, char *argv[])
{
	regex_t pat;
	int err, i;
	int opt;
	int regflags = REG_EXTENDED | REG_NEWLINE;

	mycfg.is_v = 0;

	while ((opt = getopt_long(argc, argv, "ih", longopts, NULL)) != -1) {
		switch(opt) {
		case 'i':
			regflags |= REG_ICASE;
			break;
		case 'h':
			fprintf(stdout, "Usage: %s [-i] pattern [FILE ...]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case '?':
		default:
			fprintf(stderr, "Usage: %s [-i] pattern [FILE ...]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) {
		fputs("no pattern\n", stderr);
		exit(EXIT_FAILURE);
	}

	err = regcomp(&pat, argv[optind], regflags);
	if (err != 0) {
		char buf[1024];
		regerror(err, &pat, buf, sizeof(buf));
		puts(buf);
		exit(EXIT_FAILURE);
	}
	optind++;
	if (optind == argc) {
		do_slice(&pat, stdin);
	} else {
		for (i = optind; i < argc; i++) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(EXIT_FAILURE);
			}
			do_slice(&pat, f);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(EXIT_SUCCESS);
}

static void do_slice(regex_t *pat, FILE *src)
{
	char buf[4096];
	int match;
	regmatch_t regmatch[1];

	while (fgets(buf, sizeof(buf), src)) {
		if (regexec(pat, buf, 1, regmatch, 0) == 0) {
			fwrite(buf + regmatch[0].rm_so, regmatch[0].rm_eo - regmatch[0].rm_so, 1, stdout);
			fputc('\n', stdout);
		}
	}
}
