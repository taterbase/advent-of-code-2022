/* Day 2 - Part 1
 *
 * Assess win rate of rock, paper scissor fights
 *
 * A,B,C = Opponent Rock,Paper,Scissors
 * X,Y,Z - My Rock,Paper,Scissors
 *
 * Total score is my shape (rock = 1, paper = 2, scissors = 3)
 * plus 6 if I win, 3 if I tie, and 0 if I lose
 *
 * Opponent on left, me on right
 */
#include <stdio.h>

#define BYTEWIDTH 8  /* 64 bits, because why not??? */

int main() {
	int bytesRead, score;
	char opp, me;
	char chunk[BYTEWIDTH];

	bytesRead = score = 0;

	while(1) {
		bytesRead = fread(chunk, sizeof(char), BYTEWIDTH, stdin);

		for(int i = 0; i<bytesRead; ++i) {
			switch(chunk[i]) {
				case 'A':
				case 'B':
				case 'C':
					opp = chunk[i] - '@'; // Set opposition to 1, 2 or 3
					break;
				case 'X':
				case 'Y':
				case 'Z':
					me = chunk[i] - 'W'; // Set myself to 1, 2 or 3
					score += me; // We always increment by the value of the shape

					// The math for the shapes and subtraction make it easy to see wins and ties
					switch(me - opp) {
						case 0:
							score +=3;
							break;
						case 1:
						case -2:
							score += 6;
					}
			}
		}

		if (bytesRead < BYTEWIDTH)
			break;
	}

	printf("Score: %d", score);
}
