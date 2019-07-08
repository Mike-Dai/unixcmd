/* compile: gcc hello1.c -lncurses hello1 */
#include <stdio.h>
#include <curses.h>

int main() {
	initscr();
	clear();
	move(10, 20);
	addstr("Hello, world");
	move(LINES - 1, 0);
	refresh();
	getch();
	endwin();
	return 0;
}