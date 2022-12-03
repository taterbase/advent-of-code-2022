/* Day 1 - Part 1
 *
 * process and calculate each elf's calories
 * foods separated by newlines
 * elves seprated by blank lines
 *
 * return greatest number of calories held by an elf
 */
#include <stdio.h>

#define INPUTFILE "input"

int main() {
	int c, sum, num, maxcal;
	FILE *fp;
	fp = fopen(INPUTFILE, "r");
	if (fp == NULL) {
		fputs("could not find input file\n", stderr);
		return 1;
	}

	num = sum = maxcal = 0;
	while((c = getc(fp)) != EOF) {
		if(c == '\n') {
			if (num == 0) {
				// We've finished processing an elf, see if greatest cal
				if (sum > maxcal)
					maxcal = sum;
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

	printf("Greatest calorie: %d\n", maxcal);

	return 0;
}
