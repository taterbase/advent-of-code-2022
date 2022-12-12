/*
 * Day 7 - Part 1
 */
#include <stdio.h>

#define NAMESIZE 10
#define CMDSIZE 5
#define FILENAME "input"
#define MAXNODES 1000
#define MAXSIZE 100000

#define NUL	-1
#define CMD	0
#define FLD	1
#define NUM	2
#define CD	3

struct node {
	struct node *parent;
	long size;
	char name[NAMESIZE];
};

struct nodedealer {
	char dealt;
	struct node nodes[MAXNODES];
};

struct node *getnode(struct nodedealer *nd) {
	struct node *retnode = NULL;
	if (nd->dealt >= MAXNODES) {
		printf("out of nodes bud\n");
		return retnode;
	}
	retnode = &nd->nodes[nd->dealt++];
	retnode->parent = NULL;
	retnode->size = 0;
	return retnode;
}

void broadcastsize(struct node *n, int size) {
	n->size += size;
	if (n->parent != NULL)
		broadcastsize(n->parent, size);
}

int main() {
	char cmd[CMDSIZE];
	char curname[NAMESIZE];
	char newline;
	int c, cursize, state;
	unsigned long sum;
	struct nodedealer nd = {0};
	struct node *curnode = getnode(&nd);
	FILE *f;

	state = NUL;
	newline = 1;
	cursize = sum = 0;

	f = fopen(FILENAME, "r");
	while(1) {
		c = getc(f);
		if (c == '\n') {
			newline = 1;

			switch(state) {
				case NUM:
					broadcastsize(curnode, cursize);
					cursize = 0;
					break;
			}
			state = NUL;
			continue;
		}

		if (c == EOF) {
			while(1) {
				if (curnode->size <= MAXSIZE)
					sum+=curnode->size;

				if (curnode->parent)
					curnode = curnode->parent;
				else
					break;
			}
			break;
		}

		if (newline) {
			if (c == '$') {
				state = CMD;
				// move past space to command
				getc(f);
			}
			else if (c >= '0' && c <= '9') {
				state = NUM;
				cursize = (c - '0');
			}
			newline = 0;
			continue;
		}

		if (state == CMD) {
			switch(c) {
				case 'l':
					getc(f); // gobble up s lol
					break;;
				case 'c':
					// move ahead two to get to name of
					// directory we're switching to
					state = CD;
					getc(f);
					getc(f);
					break;
			}
			continue;
		}

		if (state == NUM && c >= '0' && c <= '9') {
				cursize = (cursize * 10) + (c - '0');
				continue;
		}

		if (state == CD) {
			state = NUL;
			switch(c) {
				// We'll handle the change directory so don't worry about it later
				case '.':
					// moving on up
					if (curnode->size <= MAXSIZE)
						sum+=curnode->size;
					curnode = curnode->parent; // move up
					break;
				case '/':
					// move along
					printf("root\n");
					break;
				default:
					struct node *child = getnode(&nd);
					child->parent = curnode;
					curnode = child;
					break;
			}
			continue;
		}
	}

	fclose(f);
	printf("here is the sum %d\n", sum);
	return 0;
}
