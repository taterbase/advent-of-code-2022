/* 
 * Day 3 - Part 1
 */
#include <stdio.h>
#include <stdint.h>

#define BYTEWIDTH 8	/* 64 bits, because why not??? */
#define DISTINCT 52	/* distinct number of chars to expect */

int main() {
	int bytesRead, lineLength, seen, totalPrio;
	char letter;
	long long map, mask;

	char chunk[BYTEWIDTH];
	int firstSeen[DISTINCT];
	int lastSeen[DISTINCT];
	int appeared[DISTINCT];

	map = 0; 	// zero out bitmap
	mask = 1;	// set default for mask
	bytesRead = lineLength = seen = totalPrio = 0;

	while(1) {
		bytesRead = fread(chunk, sizeof(char), BYTEWIDTH, stdin);

		for(int i = 0; i<bytesRead; ++i, ++lineLength) {
			letter = chunk[i];
			// Reset on newline
			if (letter == '\n') {
				//printf("END OF LINE: %d %d\n", lineLength, seen);
				for (int i = 0; i < seen; ++i) {
					if ((firstSeen[appeared[i]-1] < lineLength / 2) &&
							(lastSeen[appeared[i]-1] >= lineLength / 2)) {
						//printf("WE FOUND IT\n");
						totalPrio += appeared[i];
						break;
					} 
				}
				// check length
				// iterate through characters seen and compare first/last indexes
				seen = lineLength = map = 0;
				continue;
			}

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
			if (mask & map) {
				//printf("We've already seen %c\n", chunk[i]);
				lastSeen[letter-1] = lineLength - 1;
			}
			else {
				//printf("Storing presence of %c\n", chunk[i]);
				map |= mask;
				appeared[seen++] = letter;
				firstSeen[letter-1] = lastSeen[letter-1] = lineLength - 1;
			}
			mask = 1; // Reset mask
		}

		if (bytesRead < BYTEWIDTH)
			break;
	}

	printf("Total Prio: %d\n", totalPrio);
}
