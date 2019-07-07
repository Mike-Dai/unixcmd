/* 
 * play_again2.c
 * purpose: ask if user wants another transaction
 * method: set tty into char-by-char and no-echo mode
 * set tty into no-delay mode
 * read char, return result
 * return: 0 => yes, 1 => no, 2 => timeout
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define QUESTION "Do you want another transaction"
#define TRIES 3              /* max tries */
#define SLEEPTIME 2          /* time per try */
#define BEEP putchar('\a')   /* alert user */

int get_response(char* question, int maxtries);
char get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();
void tty_mode(int how);

int main() {
	int response;
	tty_mode(0);                                /* save tty mode */
	set_cr_noecho_mode();                       /* set char-by-char mode */
	set_nodelay_mode();                         /* no input => EOF */
	response = get_response(QUESTION, TRIES);   /* get some answer */
	tty_mode(1);                                /* restore tty mode */
	return response;
}

int get_response(char* question, int maxtries) {
	int input;
	printf("%s, (y/n)?", question);
	fflush(stdout);
	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if (input == 'y')    return 0;
		if (input == 'n')    return 1;
		if (maxtries-- == 0) return 2;
		BEEP;
	}
}

char get_ok_char() {
	int c;
	while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode() {
	/* 
	 * purpose: put file descriptor 0 (i.e. stdin) into char-by-char and noecho mode
	 * method: use bits in termios
	 */
	struct termios ttystate;
	tcgetattr(0, &ttystate);          /* read current setting */
	ttystate.c_lflag &= ~ICANON;      /* no buffering */
	ttystate.c_lflag &= ~ECHO;        /* no echo either */
	ttystate.c_cc[VMIN] = 1;          /* get 1 char at a time */
	tcsetattr(0, TCSANOW, &ttystate); /* install settings */
}

void set_nodelay_mode() {
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

/* how == 0 => save current mode, how == 1 => restore mode */ 
void tty_mode(int how) {
	static struct termios original_mode;
	static int original_flags;
	if (how == 0) {
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
	}
	else {
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}