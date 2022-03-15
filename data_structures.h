#ifndef data_structures_h
#define data_structures_h

typedef struct opcodes {
	char* opcode;
	char* funct;
	char* binary_val;
	int source_operand_types[5];
	int destination_operand_types[5];
} opcodes;

typedef struct registers {
	char *register_name;
	char reg_binary[4];
} registers;


char code[8191][21];
char data[8191][21];


// extern opcodes_table;


#endif
 