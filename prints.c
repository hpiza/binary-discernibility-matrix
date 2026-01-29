#include "prints.h"
#include <stdio.h>
#include <stdlib.h>

void printBM(bmatrix* bm, size rows) {
	int R = rows >= 0 && rows < bm->rows? rows: bm->rows;
	bnode* n = bm->first;
	for(int r = 0; r < R; r ++) {
		printf("%3d) ", r + 1);
		printBytes(n->array, bm->cols);
		n = n->next;
	}
	printf("----------------------\n");
	printf("%d rows, %d columns\n", bm->rows, bm->cols);
	printf("----------------------\n");
}

void printBytes(byte* bytes, size l) {
	for(int i = 0; i < l; i ++) {
		printf("%d ", bytes[i]);
	}
	printf("\n");
}

void printNode(snode *n, size printed, size toPrint) {
	if(n == NULL || printed == toPrint) {
		printf("----------------------------\n");
		return;
	}
	printf("%3d) ", printed + 1);
	printStrings(n->array);
	printNode(n->next, printed + 1, toPrint);
}

void printStrings(sarray* array) {
	int i;
	for(i = 0; i < array->length; i ++) {
		printf("%s ", array->string[i]);
	}
	printf("\n");
}

void printTM(tmatrix* tm, size rows) {
	printNode(tm->first, 0, rows);
	printf("%d rows, %d columns\n", tm->rows, tm->cols);
	printf("----------------------------\n");
}
