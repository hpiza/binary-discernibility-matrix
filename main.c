#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prints.h"

#define BUFFER_SIZE 10000

void       display_menu();
tmatrices* create_tmatrices(tmatrix*, char* filename);
void       free_tmatrices(tmatrices*);
void       destroy_tm(tmatrix*);
tmatrices* load_from_file();
tmatrix*   load_csv(str);
sarray*    split(char[], char sep);
snode*     create_str_node(sarray*);
bool       valid_csv_filename(char*);
tmatrices* load_from_folder();
int        select_source();
tmatrices* load_data();
char* 	   get_filename(char*);
void       incremental_reducts(tmatrices*);
void       original_reducts(tmatrices*);
void       incremental_constructs(tmatrices*);
void       original_constructs(tmatrices*);
bmatrix*   create_bm_incremental(tmatrix*, bool);
bool       is_zero_row(byte*, size);
bool       is_subrow(byte*, byte*, size);
void       destroy_bm(bmatrix*);
bnode*     create_bytes_node(byte*, size);
bmatrix*   create_dm(tmatrix*, bool);
void       dm_to_bm(bmatrix*);
void	   save_bm(str, bmatrix*);
char*      itostr(int, char*);
void       fill(char[], byte*, int);

int main() {
	setbuf(stdout, NULL);
	int opc;
	tmatrices* tms = NULL;
	do {
		display_menu();
		scanf("%d", &opc);
		switch(opc) {
			case 1: free_tmatrices(tms);
			        tms = load_data();
			        break;
			case 2: incremental_reducts(tms); break;
			case 3: original_reducts(tms); break;
			case 4: incremental_constructs(tms); break;
			case 5: original_constructs(tms); break;
			case 6: printf("------------------------------------\n");
		}
		fflush(stdin);
	} while(opc != 6);
	return 0;
}

void display_menu() {
	printf("====================================\n");
	printf("MAIN MENU\n");
	printf("------------------------------------\n");
	printf("[1] Load dataset file(s)\n");
	printf("[2] Incremental algorithm for reducts\n");
	printf("[3] Original algorithm for reducts\n");
	printf("[4] Incremental algorithm for constructs\n");
	printf("[5] Original algorithm for constructs\n");
	printf("[6] Exit\n");
	printf("Select option: ");
}

void free_tmatrices(tmatrices* tms) {
	if(tms == NULL) return;
	int i;
	for (i = 0; i < tms->count; i ++) {
		destroy_tm(tms->array[i]);
	}
	free(tms->array);
	free(tms);
}

void destroy_tm(tmatrix* mat) {
	snode* n1 = mat->first;
	snode* n2;
	while(n1 != NULL) {
		n2 = n1 ->next;
		free(n1);
		n1 = n2;
	}
	free(mat);
}

tmatrices* load_data() {
	printf("====================================\n");
	printf("Load dataset file(s)\n");
	printf("------------------------------------\n");
	int src = select_source();
	tmatrices* tms = NULL;
	switch(src) {
		case 1: tms = load_from_file(); break;
		case 2: tms = load_from_folder(); break;
		case 3: printf("Data not loaded\n");
	}
	return tms;
}

int select_source() {
	int opc;
	do {
		printf("[1] File\n");
		printf("[2] Directory\n");
		printf("[3] Return\n");
		printf("Select source: ");
		scanf("%d", &opc);
		fflush(stdin);
	} while(opc != 1 && opc != 2 && opc != 3);
	return opc;
}

tmatrices* load_from_file() {
	char filename[50];
	printf("Enter file name: ");
	scanf("%s", filename);
	tmatrix* tm = load_csv(filename);
	return create_tmatrices(tm, filename);
}

tmatrix* load_csv(str filename) {
	FILE* f = fopen(filename, "r");
	if(!f) {
		printf("Unable to open the file %s\n", filename);
		return NULL;
	}
	tmatrix* tm = (tmatrix*) malloc(sizeof(tmatrix));
	tm->first = NULL, tm->last = NULL;
	tm->rows = 0;
	snode *n = NULL;
	char buffer[BUFFER_SIZE];
	while(fgets(buffer, BUFFER_SIZE, f) != NULL) {
		sarray* data = split(buffer, ',');
		if(tm->rows == 0) {
			tm->cols = data->length;
		} else {
			n = create_str_node(data);
			if(tm->first == NULL) tm->first = tm->last = n;
			tm->last->next = n;
			tm->last = n;
		}
		tm->rows ++;
	}
	tm->rows --;
	fclose(f);
	printf("File '%s' loaded successfully\n", filename);
	return tm;
}

sarray* split(char row[], char separator) {
	int i, j, k;
	int n = strlen(row);
	int m = 1;
	for(i = 0; i < n; i ++) if(row[i] == separator) m ++;
	sarray* array = (sarray*) malloc(sizeof(sarray));
	array->length = m;
	array->string = (str*) malloc(m * sizeof(str));
	str st = NULL;
	char buf[1000] = {0};
	for(i = 0, j = 0, k = 0; i < n; i ++) {
		if(row[i] == separator || row[i] == '\n') {
			buf[i - j] = '\0';
			st = (str) calloc(i - j + 1, sizeof(char));
			strcpy(st, buf);
			array->string[k ++] = st;
			j = i + 1;
		} else {
			buf[i - j] = row[i];
		}
	}
	return array;
}

snode* create_str_node(sarray* data) {
	snode* n = (snode*) malloc(sizeof(snode));
	n->array = data;
	n->next = NULL;
	return n;
}

tmatrices* create_tmatrices(tmatrix* tm, char* filename) {
	tmatrices* tms = (tmatrices*) malloc(sizeof(tmatrices));
	tms->count = 1;
	tms->array = (tmatrix**) malloc(sizeof(tmatrix*));
	tms->array[0] = tm;
	tms->filenames = (char**) malloc(sizeof(char*));
	tms->filenames[0] = (char*) malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(tms->filenames[0], filename);
	return tms;
}

tmatrices* load_from_folder() {
	char folder[50];
	printf("Enter directory name: ");
	scanf("%s", folder);
	struct dirent *dir;
	DIR *d = opendir(folder);
	tmatrices* tms = (tmatrices*) malloc(sizeof(tmatrices));
	tms->count = 0;
	while ((dir = readdir(d)) != NULL) {
		if (valid_csv_filename(dir->d_name)) {
			tms->count ++;
		}
	}
	closedir(d);
	tms->array = (tmatrix**) malloc(tms->count * sizeof(tmatrix*));
	tms->filenames = (char**) malloc(tms->count * sizeof(char*));
	d = opendir(folder);
	int i = 0;
	while ((dir = readdir(d)) != NULL) {
		char filename[100] = {0};
		strcpy(filename, folder);
		strcat(filename, "/");
		if (valid_csv_filename(dir->d_name)) {
			strcat(filename, dir->d_name);
			tms->array[i] = load_csv(filename);
			tms->filenames[i] = (char*) malloc((strlen(filename) + 1) * sizeof(char));
			strcpy(tms->filenames[i], filename);
			i ++;
		}
	}
	closedir(d);
	return tms;
}

bool valid_csv_filename(char *filename) {
	size lenf = strlen(filename);
	size lenc = strlen(".csv");
	if(lenf <= lenc) return false;
	if(!strcmp(filename + lenf - lenc, ".csv")) return true;
    return false;
}

char* get_filename(char* path) {
    char* filename = path;
    int i = 0;
    for(char* curr = path; *curr != '\0'; curr++, i ++) {
        if(*curr == '/' || *curr == '\\') {
        	filename = curr + 1;
        	i = 0;
        }
        else if(*curr == '.')
        	filename[i - 1] = '\0';
    }
    return filename;
}

void incremental_reducts(tmatrices* tms) {
	if(tms == NULL) {
		printf("You need to load at least one dataset before running any algorithm\n");
		return;
	}
	printf("====================================\n");
	printf("Incremental algorithm for Reducts\n");
	int i;
	FILE* f = fopen("log.csv", "w");
	fprintf(f, "Incremental algorithm for Reducts\n");
	fprintf(f, "dataset,rows-ma,cols-ma,rows-mb,cols-mb,seconds\n");
	bmatrix* bm;
	for(i = 0; i < tms->count; i ++) {
		char* filename = get_filename(tms->filenames[i]);
		printf("Processing %s - ", filename);
		clock_t start = clock();
		bm = create_bm_incremental(tms->array[i], true);
		clock_t end = clock();
		double delay = ((double) (end - start)) / CLOCKS_PER_SEC;
		save_bm(filename, bm);
		fprintf(f, "%s,%d,%d,%d,%d,%.2f\n", filename, tms->array[i]->rows,
				tms->array[i]->cols, bm->rows, bm->cols, delay);
		destroy_bm(bm);
		printf("ended\n");
	}
	printf("------------------------------------\n");
	fclose(f);
}

void original_reducts(tmatrices* tms) {
	if(tms == NULL) {
		printf("You need to load at least one dataset before running any algorithm\n");
		return;
	}
	printf("====================================\n");
	printf("Original Algorithm para Reductos\n");
	int i;
	FILE* f = fopen("log.csv", "w");
	fprintf(f, "Original algorithm for Reducts\n");
	fprintf(f, "dataset,rows-ma,cols-ma,rows-mb,cols-mb,seconds\n");
	bmatrix* bm;
	for(i = 0; i < tms->count; i ++) {
		char* filename = get_filename(tms->filenames[i]);
		printf("Processing %s - ", filename);
		clock_t start = clock();
		bm = create_dm(tms->array[i], true);
		dm_to_bm(bm);
		clock_t end = clock();
		double delay = ((double) (end - start)) / CLOCKS_PER_SEC;
		save_bm(filename, bm);
		fprintf(f, "%s,%d,%d,%d,%d,%.2f\n", filename, tms->array[i]->rows,
				tms->array[i]->cols, bm->rows, bm->cols, delay);
		destroy_bm(bm);
		printf("ended\n");
	}
	printf("------------------------------------\n");
	fclose(f);
}

void incremental_constructs(tmatrices* tms) {
	if(tms == NULL) {
		printf("You need to load at least one dataset before running any algorithm\n");
		return;
	}
	printf("====================================\n");
	printf("Incremental Algorithm for Constructs\n");
	int i;
	FILE *f = fopen("log.csv", "w");
	fprintf(f, "Incremental Algorithm for Constructs\n");
	fprintf(f, "dataset,rows-ma,cols-ma,rows-mb,cols-mb,seconds\n");
	bmatrix *bm;
	for (i = 0; i < tms->count; i++) {
		char *filename = get_filename(tms->filenames[i]);
		printf("Processing %s - ", filename);
		clock_t start = clock();
		bm = create_bm_incremental(tms->array[i], false);
		clock_t end = clock();
		double delay = ((double) (end - start)) / CLOCKS_PER_SEC;
		save_bm(filename, bm);
		fprintf(f, "%s,%d,%d,%d,%d,%.2f\n", filename, tms->array[i]->rows,
				tms->array[i]->cols, bm->rows, bm->cols, delay);
		destroy_bm(bm);
		printf("ended\n");
	}
	printf("------------------------------------\n");
	fclose(f);
}

void original_constructs(tmatrices *tms) {
	if (tms == NULL) {
		printf("You need to load at least one dataset before running any algorithm\n");
		return;
	}
	printf("====================================\n");
	printf("Original Algorithm for Constructs\n");
	int i;
	FILE *f = fopen("log.csv", "w");
	fprintf(f, "Original Algorithm para Constructs\n");
	fprintf(f, "dataset,rows-ma,cols-ma,rows-mb,cols-mb,seconds\n");
	bmatrix *bm;
	for (i = 0; i < tms->count; i++) {
		char *filename = get_filename(tms->filenames[i]);
		printf("Processing %s - ", filename);
		clock_t start = clock();
		bm = create_dm(tms->array[i], false);
		dm_to_bm(bm);
		clock_t end = clock();
		double delay = ((double) (end - start)) / CLOCKS_PER_SEC;
		save_bm(filename, bm);
		fprintf(f, "%s,%d,%d,%d,%d,%.2f\n", filename, tms->array[i]->rows,
				tms->array[i]->cols, bm->rows, bm->cols, delay);
		destroy_bm(bm);
		printf("ended\n");
	}
	printf("------------------------------------\n");
	fclose(f);
}

bmatrix* create_bm_incremental(tmatrix* tm, bool reducts) {
	size R = tm->rows, C = tm->cols - 1;
	bmatrix* bm = (bmatrix*) malloc(sizeof(bmatrix));
	bm->rows = 0;
	bm->cols = C;
	bm->first = bm->last = NULL;
	byte cache[C];

	size r1, r2 = 0, c;
	snode *row1 = tm->first, *row2;
	bnode* node = 0;
	bool outOfMemory = false;
	for (r1 = 0; r1 < R - 1; r1++) {
		row2 = row1->next;
		for (r2 = r1 + 1; r2 < R; r2++) {
			for (c = 0; c < C; c++) {
				cache[c] = 0;
				if(reducts) {
					if (strcmp(row1->array->string[C], row2->array->string[C]) != 0 &&
						strcmp(row1->array->string[c], row2->array->string[c]) != 0) cache[c] = 1;
				} else {
					if (strcmp(row1->array->string[C], row2->array->string[C]) == 0 &&
						strcmp(row1->array->string[c], row2->array->string[c]) == 0) cache[c] = 1;
				}
			}
			row2 = row2->next;
			if(is_zero_row(cache, C)) continue;
			bnode* node1 = bm->first, *node2 = NULL;
			bool foundSubRow = false;
			while(!foundSubRow && node1 != NULL) {
				bnode* next = node1->next;
				if(is_subrow(cache, node1->array, C)) {
					if(!node2) {
						bm->first = next;
					} else {
						node2->next = next;
						if(node1 == bm->last) bm->last = node2;
					}
					free(node1->array);
					free(node1);
					bm->rows --;
				}
				else {
					if(is_subrow(node1->array, cache, C)) foundSubRow = true;
					node2 = node1;
				}
				node1 = next;
			}
			if(!foundSubRow) {
				// bm->last pudo desaparecer
				node = create_bytes_node(cache, C);
				if(!node) {
					outOfMemory = true;
					goto finish;
				}
				if(!bm->first) bm->first = node;
				else bm->last->next = node;
				bm->last = node;
				bm->rows ++;
			}
		}
		row1 = row1->next;
	}
	finish: if(outOfMemory) printf("Out of memory! The created BM is incomplete - \n");
	return bm;
}

bool is_zero_row(byte* row, size C) {
	for(int i = 0; i < C; i ++) if(row[i] != 0) return false;
	return true;
}

bool is_subrow(byte* row1, byte* row2, size C) {
	for (int c = 0; c < C; c++) {
		if (row1[c] == 1 && row2[c] == 0) return false;
	}
	return true;
}

void destroy_bm(bmatrix* mat) {
	bnode *n1 = mat->first;
	bnode *n2;
	while (n1 != NULL) {
		n2 = n1 ->next;
		free(n1);
		n1 = n2;
	}
	free(mat);
}

bnode* create_bytes_node(byte* cache, size C) {
	bnode* node = (bnode*) malloc(sizeof(bnode));
	if(node == NULL) return NULL;
	node->array = (byte*) malloc(C * sizeof(byte));
	if(node->array == NULL) return NULL;
	for(int c = 0; c < C; c ++) node->array[c] = cache[c];
	node->next = NULL;
	return node;
}

bmatrix* create_dm(tmatrix* tm, bool reducts) {
	size R = tm->rows, C = tm->cols - 1;
	bmatrix* bm = (bmatrix*) malloc(sizeof(bmatrix));
	bm->rows = 0;
	bm->cols = C;
	bm->first = bm->last = NULL;
	byte cache[C];

	size r1, r2, c;
	snode *row1 = tm->first, *row2;
	bnode* node = NULL;
	for (r1 = 0; r1 < R - 1; r1++) {
		row2 = row1->next;
		for (r2 = r1 + 1; r2 < R; r2++) {
			for (c = 0; c < C; c++) {
				cache[c] = 0;
				if (reducts) {
					if (strcmp(row1->array->string[C], row2->array->string[C]) != 0 &&
						strcmp(row1->array->string[c], row2->array->string[c]) != 0)
						cache[c] = 1;
				} else {
					if (strcmp(row1->array->string[C], row2->array->string[C]) == 0 &&
						strcmp(row1->array->string[c], row2->array->string[c]) == 0)
						cache[c] = 1;
				}
			}
			if(!is_zero_row(cache, C)) {
				node = create_bytes_node(cache, C);
				if(!node) goto finish;
				if(bm->first == NULL) bm->first = bm->last = node;
				else bm->last->next = node;
				bm->last = node;
				bm->rows ++;
			}
			row2 = row2->next;
		}
		row1 = row1->next;
	}
	finish: if(!node) printf("Out of memory! The created DM is incomplete - \n");
	return bm;
}

void dm_to_bm(bmatrix* dm) {
	bnode *curr = dm->first, *prior = NULL;
	// delete super rows
	curr = dm->first;
	prior = NULL;
	while(curr != NULL) {
		bnode* next = curr->next;
		bool found = false;
		bnode* n = dm->first;
		while(n != NULL) {
			if(n != curr && is_subrow(n->array, curr->array, dm->cols)) {
				found = true;
				break;
			}
			n = n->next;
		}
		if(found) {
			if(prior == NULL) dm->first = curr->next;
			else prior->next = curr->next;
			free(curr->array);
			free(curr);
			dm->rows --;
		} else {
			prior = curr;
		}
		curr = next;
	}
}

void save_bm(str dataset, bmatrix* bm) {
	char bmfile[100] = "bol/";
	strcat(bmfile, dataset);
	strcat(bmfile, ".mb");
	FILE* f = fopen(bmfile, "w");
	if(!f) {
		printf("Unable to create %s. Make sure to create a folder called /bol.\n", bmfile);
		return;
	}
	char buf[10];
	itostr(bm->rows, buf);
	fputs(buf, f);
	fputs("\n", f);
	itostr(bm->cols, buf);
	fputs(buf, f);
	fputs("\n", f);
	char buffer[BUFFER_SIZE];
	bnode* curr = bm->first;
	while(curr != NULL) {
		fill(buffer, curr->array, bm->cols);
		fputs(buffer, f);
		curr = curr->next;
	}
	fclose(f);
}

char *itostr(int value, char *result) {
    char *ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;
    do {
        tmp_value = value;
        value /= 10;
        *ptr++ = "9876543210123456789"[9 + (tmp_value - value * 10)];
    } while (value);
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void fill(char to[], byte* from, int C) {
	to[0] = '\0';
	int i;
	for(i = 0; i < C; i ++) {
		if(from[i]) strcat(to, "1 ");
		else strcat(to, "0 ");
	}
	strcat(to, "\n");
}
