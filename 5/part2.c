/* 
 * Day 5 - Part 2
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
	if(top->down == NULL) {
		top->down = new;
		new->up = top;
	} else
		appendbox(top->down, new);

}

void printstack(struct box *top) {
	if (top == NULL)
		return;
	printf("[%c]\n", top->value);
	if (top->inverted && top->up != NULL)
		printstack(top->up);
	else if (!top->inverted && top->down != NULL)
		printstack(top->down);
}

// Move as a stack, not accordian
void move(struct box *stacks[], int amount, int from, int to) {
	// I'm just going to pass the stack numbers and let this
	// function worry about turning them into indexes
	from = from - 1;
	to = to - 1;

	struct box *leader = stacks[from];
	struct box *acceptor = stacks[to];
	struct box *tail = leader;

	for (int i=1; i<amount; ++i) {
		tail = tail->down;
	}

	struct box *oldbase = tail->down;
	tail->down = acceptor;
	if (acceptor != NULL)
		acceptor->up = tail;

	stacks[to] = leader;
	stacks[from]=oldbase;
}

int main() {
	char c, stackbuilt, curstack, boxesused, newline, blanks, stacknum;
	struct box *stacks[MAXSTACK];
	struct boxpool pool = {0};

	FILE *f;
	f = fopen("input", "r");

	for(int i=0;i<MAXSTACK;++i)
		stacks[i] = NULL;

	curstack = stackbuilt = boxesused = newline = blanks = stacknum = 0;

	while(!stackbuilt) {
		c = getc(f);
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

	printf("Stack built %d\n", stacknum);
	printf("Before:\n");
	for(int i = 0; i<stacknum; ++i) {
		printf("Stack %d:\n", i);
		printstack(stacks[i]);
	}

	char moving = 1;
	char numidx, isnum = 0;
	char nums[3] = {0, 0, 0}; //amount, from, to
	while(moving) {
		c = getc(f);
		if (c >= '0' && c <= '9') {
			isnum = 1;
			nums[numidx] = (nums[numidx] * 10) + (c - '0');
		} else if (isnum) {
			++numidx;
			isnum = 0;
		}

		if (c == '\n') {
			printf("moving %d %d %d\n", nums[0], nums[1], nums[2]);
			move(stacks, nums[0], nums[1], nums[2]);
			nums[0] = nums[1] = nums[2] = 0;
			numidx = 0;
		}
		if (c == EOF)
			break;
	}

	printf("After:\n");
	for(int i = 0; i<stacknum; ++i) {
		//printf("Stack %d:\n", i);
		//printstack(stacks[i]);
		printf("%c", stacks[i]->value);
	}

	fclose(f);
}
