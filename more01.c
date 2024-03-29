#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE* fp);

int see_more(FILE* cmd);

int main(int argc, char* argv[]) {
	FILE* fp;
	if (argc == 1) {
		do_more(stdin);
	}
	else {
		while (--argc) {
			if ((fp = fopen(*++argv, "r")) != NULL) {
				do_more(fp);
				fclose(fp);
			}
			else {
				exit(1);
			}
		}
	}
	return 0;
}

void do_more(FILE* fp) {
	char line[LINELEN];
	int see_more(), reply;
	int num_of_lines = 0;
	FILE* fp_tty;
	fp_tty = fopen("/dev/tty", "r");
	if (fp_tty == NULL) {
		exit(1);
	}
	while (fgets(line, LINELEN, fp)) {
		if (num_of_lines == PAGELEN) {
			reply = see_more(fp_tty);
			if (reply == 0) {
				break;
			}
			num_of_lines -= reply;
		}
		if (fputs(line, stdout) == EOF) {
			exit(1);
		}
		num_of_lines++;
	}
}

int see_more(FILE* cmd) {
	int c;
	printf("\033[7m more? \033[m");
	while ((c = getc(cmd)) != EOF) {
		if (c == ' ') {
			return PAGELEN;
		}
		if (c == '\n') {
			return 1;
		}
		if (c == 'q') {
			return 0;
		}
	}
	return 0;
}