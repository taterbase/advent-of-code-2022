/* Day 2 - Part 2
 *
 * Assess win rate of rock, paper scissor fights
 *
 * A,B,C = Opponent Rock,Paper,Scissors
 * X,Y,Z - Lose, Draw, Win
 *
 * Total score is my shape (rock = 1, paper = 2, scissors = 3)
 * plus 6 if I win, 3 if I tie, and 0 if I lose
 *
 * Opponent on left, win condition on right
 */
#include <stdio.h>

#define BYTEWIDTH 8  /* 64 bits, because why not??? */

int main() {
	int bytesRead, score;

	/* 
	 * win and loss list reflect indexes for the corresponding
	 * play when the opponent plays a move
	 * rock(0) win = 2(paper), loss = 3(scissor)
	 */
	char win[] = {2, 3, 1};
	char lose[] = {3, 1, 2};
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
					opp = chunk[i] - '@';	/* Set opponent value to 
																	 1, 2, 3 (rock, paper, scissor) */
					break;
				case 'X': 							// LOSE
					score += lose[opp-1];
					break;
				case 'Y':								// TIE
					score +=3;
					score += opp;
					break;
				case 'Z':								// WIN
					score += 6;
					score += win[opp-1];
					break;
			}
		}

		if (bytesRead < BYTEWIDTH)
			break;
	}

	printf("Score: %d", score);
}
