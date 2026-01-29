#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned int size;
typedef enum {false, true} bool;
typedef char* str;
typedef unsigned char byte;

typedef struct structbnode {
	struct structbnode* next;
	byte* array;
} bnode;

typedef struct {
	bnode *first, *last;
	size rows, cols;
} bmatrix;

typedef struct {
	str* string;
	size length;
} sarray;

typedef struct structsnode {
	struct structsnode* next;
	sarray* array;
} snode;

typedef struct {
	snode *first, *last;
	size rows, cols;
} tmatrix;

typedef struct {
	unsigned int count;
	tmatrix** array;
	char** filenames;
} tmatrices;

#endif /* TYPES_H_ */
