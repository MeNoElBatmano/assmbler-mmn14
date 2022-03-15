#ifndef data_structures.h
#define data_structures.h

typedef struct opCode {
    char *op;
    char *value;
    
}

typedef struct symbol_table {
    char *symbol;
    int value;
    int base_adress;
    int offset_address;
    ?? atrributes;
}