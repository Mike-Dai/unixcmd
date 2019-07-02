#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SHOWHOST

void show_info(struct utmp*);
void showtime(long);

int main() {
	struct utmp current_record;  /* read info into here */
	int    utmpfd;               /* read from this descriptor */ 
	int    reclen = sizeof(current_record);

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}

	while (read(utmpfd, &current_record, reclen) == reclen) {
		show_info(&current_record);
	}
	close(utmpfd);
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

/* marks a utmp record as logged out
does not blank username or remote host
return -1 on error, 0 on success
*/
int logout_tty(char* line) {
	int fd;
	struct utmp rec;
	int len = sizeof(struct utmp);
	int retval = -1;

	if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
		return -1;
	}
	/* search and replace */
	while (read(fd, &rec, len) == len) {
		if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0) {
			rec.ut_type = DEAD_PROCESS;                  /* set type */
			if (time(&rec.ut_time) != -1) {  //???????
				if (lseek(fd, -len, SEEK_CUR) != -1) {   /* back up */
					if (write(fd, &rec, len) == len) {   /* update */
						retval = 0;                      /* success */
					}
				}
			}
			break;  
		}
	}
	/* close the file */
	if (close(fd) == -1) {
		retval = -1;
	}
	return retval;
}