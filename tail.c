#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#define LINE_LIMIT 10
#define BUFFER_SIZE 1024

typedef struct line {
	char *bufp;
	struct line *next;
} line_t;

static void do_tail(FILE *f);

int main(int argc, char *argv[])
{
	do_tail(stdin);
	exit(EXIT_SUCCESS);
}

static void do_tail(FILE *f)
{
	int c;
	int cnt = 0;
	char *buf = NULL;
	char *buftmp = NULL;
	int bufsize = BUFFER_SIZE;
	line_t *head = NULL;
	line_t *curr = NULL;
	line_t *tmp = NULL;
	int linecnt = 0;

	head = (line_t *)malloc(sizeof(line_t));
	if (!head) {
		perror("Failed to allocate head");
		exit(EXIT_FAILURE);
	}
	head->bufp = NULL;
	head->next = NULL;
	curr = head;

	buf = (char *)malloc(bufsize);
	if (!buf) {
		perror("Failed to allocate buf");
		exit(EXIT_FAILURE);
	}

	while((c = fgetc(f)) != EOF) {
		buf[cnt] = c;
		cnt++;
		if (cnt > BUFFER_SIZE - 1) {
			bufsize = bufsize + BUFFER_SIZE;
			buftmp = buf;
			buf = (char *)malloc(bufsize);
			if (!buf) {
				perror("Failed to allocate buf");
				exit(EXIT_FAILURE);
			}
			memcpy(buf, buftmp, bufsize - BUFFER_SIZE);
			free(buftmp);
		}
		if (c == '\n') {
			buf[cnt] = '\0';
			curr->bufp = buf;

			linecnt++;
			if (linecnt > LINE_LIMIT) {
				tmp = head;
				head = head->next;
				free(tmp->bufp);
				free(tmp);
			}

			curr->next = (line_t *)malloc(sizeof(line_t));
			if (!curr->next) {
				perror("Failed to allocate next");
				exit(EXIT_FAILURE);
			}
			curr->next->bufp = NULL;
			curr->next->next = NULL;
			curr = curr->next;

			cnt = 0;
			bufsize = BUFFER_SIZE;
			buf = (char *)malloc(bufsize);
			if (!buf) {
				perror("Failed to allocate buf");
				exit(EXIT_FAILURE);
			}
		}
	}

	curr = head;

	while (1) {
		if (curr->bufp) printf("%s", curr->bufp);
		curr = curr->next;
		if (curr == NULL) break;
	}
}
