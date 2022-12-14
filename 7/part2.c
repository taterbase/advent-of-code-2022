/*
 * Day 7 - Part 1
 */
#include <stdio.h>

#define NAMESIZE 10
#define MAXNODES 1000
#define DISKSPACE 70000000
#define SPACENEEDED 30000000
#define FILENAME "input"
#define SIZETOREMOVE 572957
//#define SIZETOREMOVE 8381165
//#define FILENAME "test"

#define NUL	-1
#define CMD	0
#define FLD	1
#define NUM	2
#define CD	3

struct node {
	struct node *parent;
	unsigned long size;
	char name[NAMESIZE];
};

struct nodedealer {
	int dealt;
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

void broadcastsize(struct node *n, unsigned long size) {
	n->size += size;
	if (n->parent != NULL)
		broadcastsize(n->parent, size);
}

int main() {
	char curname[NAMESIZE];
	char newline;
	int c, state, nameidx;
	unsigned long cursize;
	struct nodedealer nd = {0};
	struct node *curnode = getnode(&nd);
	struct node *bestnode = NULL;
	FILE *f;

	state = NUL;
	newline = 1;
	cursize = nameidx = 0;

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
				case CD:
					struct node *child = getnode(&nd);
					for (int i = 0; i < nameidx; ++i)
						child->name[i] = curname[i];
					child->name[nameidx]='\0';
					nameidx = 0;
					child->parent = curnode;
					curnode = child;
					break;
			}
			state = NUL;
			continue;
		}

		if (c == EOF) {
			printf("end of the line %d\n", cursize);
			while(1) {
				if (curnode->size >= SIZETOREMOVE) {
					if (bestnode == NULL || curnode->size < bestnode->size) {
						printf("setting best node %s %d\n", curnode->name, curnode->size);
						bestnode = curnode;
					}
				} else {
					printf("not the best %s %d\n", curnode->name, curnode->size);
				}

				if (curnode->parent)
					curnode = curnode->parent;
				else
					break;
			}
			long taken, remove, avail, delta;
			avail = DISKSPACE - curnode->size;
			delta = SPACENEEDED - avail;

			printf("Root space taken %d\n", curnode->size);
			printf("space needed %d\n", SPACENEEDED);
			printf("space available %d\n", avail);
			printf("space to remove %d\n", delta);

			//printf("Space left %d\n", DISKSPACE - curnode->size);
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
			switch(c) {
				// We'll handle the change directory so don't worry about it later
				case '.':
					state = NUL;
					if (curnode->size >= SIZETOREMOVE) {
						if (bestnode == NULL || curnode->size < bestnode->size) {
							printf("setting best node %s %d\n", curnode->name, curnode->size);
							bestnode = curnode;
						}
					} else {
						printf("not the best %s %d\n", curnode->name, curnode->size);
					}

					curnode = curnode->parent; // move up
					break;
				case '/':
					state = NUL;
					// move along
					printf("root\n");
					break;
				default:
					curname[nameidx++] = c;
					break;
			}
			continue;
		}
	}

	fclose(f);
	printf("best node %s %d\n", bestnode->name, bestnode->size);
	return 0;
}
