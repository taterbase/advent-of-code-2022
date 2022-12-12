/* 
 * Day 6 - Part 1
 *
 * Detect the first occurrence of a string
 * of 4 unique characters.
 *
 */
#include <stdio.h>

#define MAXLENGTH 4
#define BLOCKQUOTA 5
#define FILENAME "input"

struct block {
	char c;
	struct block *head;
	struct block *tail;
};

struct window {
	struct block *front;
	struct block *back;
	short count;
	short uniqs;
	short processed;
	short maxlength;
	char map[26];
};

struct blockdealer {
	short dealt;
	struct block blocks[BLOCKQUOTA];
};

struct block *getblock(struct blockdealer *bd) {
	if(bd->dealt >= BLOCKQUOTA) {
		printf("Out of blocks brother\n");
		return NULL;
	}
	struct block *b = &bd->blocks[bd->dealt++];
	b->head = NULL;
	b->tail = NULL;
	b->c = '\0';
	return b;
}

void printwindow(struct window *w) {
	struct block *b = w->front;
	while(b != NULL) {
		printf("%c", b->c);
		b = b->tail;
	}
	printf("\n");
}

// hash char to fit in 26 length array
char key(char c) {
	return c - 'a';
}

// push returns a block in the event it
// has to pop one off to make room
struct block *push(struct window *w, struct block *b) {
	struct block *rblock = NULL;
	++w->processed;

	if (w->count >= w->maxlength) {
		rblock = w->back;
		w->back = rblock->head;
		w->back->tail = NULL;
		rblock->head = NULL;
	} else
		w->count++;

	struct block *oldfront = w->front;
	w->front = b;
	if (oldfront != NULL) {
		b->tail = oldfront;
		oldfront->head = b;
	}

	if (w->count == 1)
		w->back = b;

	if (rblock != NULL && w->map[key(rblock->c)]) {
		printf("removing %c\n", rblock->c);
		if(--w->map[key(rblock->c)] == 0)
			w->uniqs--;
	}

	if(++w->map[key(b->c)] == 1)
		++w->uniqs;

	return rblock;
}

int main() {
	int c;
	struct block *curblock = NULL;
	struct blockdealer bd = {0};
	struct window win = {NULL, NULL, 0, 0, 0, MAXLENGTH, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	FILE *f;

	f = fopen(FILENAME, "r");
	while(1) {
		c = getc(f);
		if (c == EOF) {
			printf("end of file\n");
			break;
		} else if (c == '\n')
			continue;

		// if block is null get a block
		if (!curblock)
			curblock = getblock(&bd);
		// push char onto window
		curblock->c = c;
		// assign block to return from push onto window
		curblock = push(&win, curblock);
		printwindow(&win);
		if (win.uniqs == 4) {
			printf("FOUND IT AT %d\n", win.processed);
			break;
		}
	}
	fclose(f);
}
