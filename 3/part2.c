/* 
 * Day 3 - Part 2
 */
#include <stdio.h>
#include <stdint.h>

#define BYTEWIDTH 8	/* 64 bits, because why not??? */
#define DISTINCT 52	/* distinct number of chars to expect */

int main() {
	int bytesRead, totalPrio, groupLine, foundBadge;
	char letter;
	long long map, lastMap, mask;

	char chunk[BYTEWIDTH];
	int appeared[DISTINCT];

	map = lastMap = 0; 	// zero out bitmap
	mask = 1;						// set default for mask

	bytesRead = totalPrio = groupLine = foundBadge = 0;

	while(1) {
		bytesRead = fread(chunk, sizeof(char), BYTEWIDTH, stdin);

		for(int i = 0; i<bytesRead; ++i) {
			letter = chunk[i];
			// Reset on newline
			if (letter == '\n') {
				if (groupLine == 2)
					lastMap = map = foundBadge = 0;
				else {
					lastMap = map;
					map = 0;
				}

				groupLine = (groupLine + 1) % 3;
				continue;
			} else if (foundBadge)
				continue;

			// A-Z need to represent 27-52, adjust
			if (letter >= 'A' && letter <= 'Z')
				letter = letter - 38;
			// a-z need to represent 1-26, adjust
			else if (letter >= 'a' && letter <= 'z')
				letter = letter - 96;
			// not an ascii char, continue
			else
				continue;

			mask <<= letter;
			switch(groupLine) {
				case 0:
					map |= mask;
					break;
				case 1:
					if (lastMap & mask)
						map |= mask;
					break;
				case 2:
					if (lastMap & mask) {
						totalPrio += letter;
						foundBadge = 1;
					}
					break;
			}
			mask = 1; // Reset mask
		}

		if (bytesRead < BYTEWIDTH)
			break;
	}

	printf("Total Prio: %d\n", totalPrio);
}
