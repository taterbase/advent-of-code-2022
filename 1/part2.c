/* Day 1 - Part 2
 *
 * process and calculate each elf's calories
 * foods separated by newlines
 * elves seprated by blank lines
 *
 * Return top 3 calories
 */
#include <stdio.h>

#define INPUTFILE "input"
#define NUMOFATELVES 3

int main() {
	int c, sum, num, new, old, processing;
	int fatelves[NUMOFATELVES];
	FILE *fp;
	fp = fopen(INPUTFILE, "r");
	if (fp == NULL) {
		fputs("could not find input file\n", stderr);
		return 1;
	}

	num = sum = old = new = 0;
	for (int i = 0; i<NUMOFATELVES; ++i)
		fatelves[i] = 0;

	while(processing) {
		c = getc(fp);

		if (c == EOF)
			processing = 0;

		if(c == '\n' || c == EOF) {
			if (num == 0) {
				// We've finished processing an elf, see if it is greater
				// than any of the fat elves
				//
				// We'll always keep the skinniest of the fatelves at the front
				// of the arr, check to see if we at least the same size, if so
				// work backwards in the arr to shift the elves down appropriately
				if (sum >= fatelves[0]) {
					new = sum;
					for(int j=NUMOFATELVES-1; j>=0; --j) {
						if(new >= fatelves[j]) {
							old = fatelves[j];
							fatelves[j] = new;
							new = old;
						}
					}

				}
				sum = 0;
				continue;
			}
			// At the end of a calorie, add it to the sum and reset num
			sum += num;
			num = 0;
		} else if (c >= '0' && c <= '9') {
			num = (num * 10) + (c - '0');
		}
	}
	fclose(fp);

	sum = 0;
	for (int i = 0; i <= NUMOFATELVES; ++i)
		sum += fatelves[i];
	printf("Total calories from fattest elves: %d\n", sum);

	return 0;
}
