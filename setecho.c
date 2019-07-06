/* setecho.c
 * usage: setecho [y|n]
 * shows: how to read, change, reset tty attributes
 */
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

#define oops(s,x) { perror(s); exit(x); }

int main(int argc, char* argv[]) {
	struct termios info;
	if (argc == 1) exit(0);

	if (tcgetattr(0, &info) == -1) {           /* get attributes */
		oops("tcgetattr", 1);
	}

	if (argv[1][0] == 'y') {
		info.c_lflag |= ECHO;                  /* turn on bit */
	}
	else {
		info.c_lflag &= ~ECHO;                 /* turn off bit */
	}

	if (tcsetattr(0, TCSANOW, &info) == -1) {  /* set attributes */
		oops("tcsetattr", 2);
	}
	return 0;
}