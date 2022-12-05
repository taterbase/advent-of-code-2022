/* 
 * Day 5 - Part 1
 */
#include <stdio.h>

#define BOXBUDGET 100
#define MAXSTACK 10

struct box {
	char value;
	struct box *up;
	struct box *down;
	char inverted;
};

struct boxpool {
	char used;
	struct box boxes[BOXBUDGET];
};

struct box *getbox(struct boxpool *p) {
	if (p->used < BOXBUDGET) {
		struct box *b = &p->boxes[p->used++];
		b->inverted = 0;
		b->up = NULL;
		b->down = NULL;
		return b;
	}
	return NULL;
}

void appendbox(struct box *top, struct box *new) {
	if(top->down == NULL)
		top->down = new;
	else
		appendbox(top->down, new);
}

void printstack(struct box *top) {
	printf("[%c]\n", top->value);
	if (top->down != NULL)
		printstack(top->down);
}

int main() {
	char c, stackbuilt, curstack, boxesused, newline, blanks, stacknum;
	struct box *stacks[MAXSTACK];
	struct boxpool pool = {0};

	for(int i=0;i<MAXSTACK;++i)
		stacks[i] = NULL;

	curstack = stackbuilt = boxesused = newline = blanks = stacknum = 0;

	while(!stackbuilt) {
		c = getchar();
		if (c == '\n') {
			// Keep track of the number of stacks we have
			if (curstack > stacknum)
				stacknum = curstack;

			// There's a known blank space between the stacks and the moves
			if (newline)
				stackbuilt = 1;
			else {
				newline = 1;
				curstack = 0; // reset stack pointer
			}
			continue;
		} 

		if (c == ' ' && ++blanks >= 4) {
			++curstack;
			blanks = 0;
		}

		// We know we're progressing beyond a new line
		newline = 0;

		if (c >= 'A' && c <= 'Z') {
			blanks = 0; // reset blanks
			struct box *b = getbox(&pool);
			if (b == NULL) {
				printf("Ran out of boxes\n");
				return 1;
			}
			b->value = c;

			if (stacks[curstack] == NULL)
				stacks[curstack] = b;
			else
				appendbox(stacks[curstack], b);

			++curstack;
		}

	}

	printf("Stack built\n");

	for(int i = 0; i<stacknum; ++i) {
		printf("Stack %d:\n", i);
		printstack(stacks[i]);
	}
}
