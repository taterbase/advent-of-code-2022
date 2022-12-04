/*
 * Day 4 - Part 2
 */
#include <stdio.h>

int main() {
	int c, curnum, total, itemno;
	int set[4];

	curnum = total = itemno = 0;

	while((c = getchar()) != EOF) {
		// Just ignore carriage return
		if (c == '\r')
			continue;

		// If an integer add it on
		if (c >= '0' && c <= '9') {
			curnum = (10 * curnum) + (c - '0');
			continue;
		} else { // otherwise store what integer we have
			set[itemno++] = curnum;
			curnum = 0;
		}

		if (c == '\n') {
			if ((set[0] >= set[2] && set[0] <= set[3]) ||
					(set[1] >= set[2] && set[1] <= set[3]) ||
					(set[2] >= set[0] && set[2] <= set[1]) ||
					(set[3] >= set[0] && set[3] <= set[1]))
				++total;
			curnum = itemno = 0;
			continue;
		}
	}

	printf("Total work overlapped: %d\n", total);
}
