#ifndef first_pass_h
#define first_pass_h

#include <string.h>
#include <stdio.h>


typedef struct label_node {
    char *name;
    int value;
    int base;
    int offset;
    char *attr;
    struct label_node *next;
} label_node;

typedef struct label_table {
    int count;
    struct label_node *head;
} label_table;

label_node *create_new_node(char *name,int value, int base, int offset, char *attr);
void add_node(label_node *node,label_table *table);


char *second_word(char tokens[81][81], char *op_name);

char *op_check(char *op_name);
char* convert_to_bin_immidiate(int n);
int funct_flag_check(char* token);
void first_pass(FILE *fp, char *file_name);
void handle_label(char* copy_line, label_node *node, label_table *table, int is_data, int IC);
void line_to_tokens(char *line , char *line_i,label_table *table );
void handle_instruction(char tokens[81][81], char *op_name);
void to_binary(char tokens[81][81], int is_op, char *op_name, int dir_i);
char* convert_to_bin(int n);
char *source_adress_check(char *token);
char *source_check(char *token);
char *funct_check(char *op_name);
char *op_check(char *op_name);
int reg_check(char *token);

char* find_label(char* address);
void add_entry(char* label, label_table *table);
void second_pass(FILE *fp, char *file_name, label_table *table);
void handle_second_pass(char *token,label_table *table );
char* convert_to_bin_direct(int n);
char* convert_to_bin_extern(int n);
int to_hex(char *word_group);
void print_ps();



#endif
