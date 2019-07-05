/* spwd.c: a simplified version of pwd
 * 
 * starts in current directory and recursively
 * climbs up to root of filesystem, print top part
 * then print current part
 *
 * use readdir() to get info about each thing
 *
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

ino_t get_inode(char*);
void printpathto(ino_t);
void inum_to_name(ino_t, char*, int);

int main() {
	printpathto(get_inode("."));            /* print path to here */
	putchar('\n');							/* then add new line */						
	return 0;
}

void printpathto(ino_t this_node) {
	ino_t my_inode;
	char its_name[BUFSIZ];

	if (this_node != get_inode("..")) {
		chdir("..");                                /* up one dir */
		inum_to_name(this_node, its_name, BUFSIZ);  /* get its name */
		my_inode = get_inode(".");                  /* print head */
		printpathto(my_inode);					    /* recursively */
		printf("/%s", its_name);                    /* now print name of this */
	}
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen) {
	DIR *dir_ptr;                        /* the directory */ 
	struct dirent *direntp;              /* each entry */
	dir_ptr = opendir(".");
	if (dir_ptr == NULL) {
		perror(".");
		exit(1);
	} 
	while ((direntp = readdir(dir_ptr)) != NULL) {
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen - 1] = '\0';  /* just in case */
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
	exit(1);
}

ino_t get_inode(char *fname) {
	struct stat info;
	if (stat(fname, &info) == -1) {
		fprintf(stderr, "Cannot stat\n");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}