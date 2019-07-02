/* who3.c - who with buffered reads
* - surpresses empty records
* - formats time nicely
* - buffers input (using utmplib)
*/

#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "utmplib.c"

#define SHOWHOST

void show_info(struct utmp*);
void showtime(time_t);

int main() {
	struct utmp *utbufp, /* holds pointer to next rec */
	*utmp_next();        /* returns pointer to next */  

	if (utmp_open(UTMP_FILE) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}

	while ((utbufp = utmp_next()) != ((struct utmp*) NULL)) {
		show_info(utbufp);
	}
	utmp_close();
	return 0;
}

/* display the contents of the utmp struct */
void show_info(struct utmp* utbufp) {
	if (utbufp->ut_type != USER_PROCESS) {
		return;
	}
	printf("%-8.8s", utbufp->ut_name);  /* the logname */
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);  /* the tty */
	printf(" ");
	showtime(utbufp->ut_time);          /* display time */
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);    /* the host */
#endif
	printf("\n");
}

/* display time in a format fit for human consumption */
void showtime(long timeval) {
	char* cp;
	cp = ctime(&timeval);
	printf("%12.12s", cp+4);
}