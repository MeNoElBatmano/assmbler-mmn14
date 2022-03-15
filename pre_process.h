#ifndef pre_process_h
#define pre_process_h

#include <string.h>
#include <stdio.h>


typedef struct macro_node {
    char *name;
    char *data;
    struct macro_node *next;
} macro_node;

typedef struct macro_table {
    int count;
    struct macro_node *head;
} macro_table;



void handle_macors(FILE *fp,  macro_node *node,   macro_table *table );
void pre_processor(FILE *fp, char *filename);
void save_macro(char *macro_i,FILE *fp, char *current_line,  macro_node *node,   macro_table *table );
void print_new_file(FILE *fp,  macro_table *table , char *file_name);





#endif
