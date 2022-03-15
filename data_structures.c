#include <stdlib.h>

#include <data_structures.h>

opcodes opcodes_table[] = { 
	{ "mov", "0000",{0,1,2,3,},{1,2,3}},
	{ "cmp", "0001",{0,1,2,3},{0,1,2,3}},
	{ "add", "0010",{0,1,2,3},{1,2,3}},
	{ "sub", "0011",{0,1,2,3},{1,2,3,}},
	{ "not", "0100",{-1},{1,2,3}},
	{ "clr", "0101",{-1},{1,2,3}},
	{ "lea", "0110",{1,2},{1,2,3}},
	{ "inc", "0111",{-1},{1,2,3}},
	{ "dec", "1000",{-1},{1,2,3}},
	{ "jmp", "1001",{-1},{1,2}},
	{ "bne", "1010",{-1},{1,2}},
	{ "red", "1011",{-1},{1,2,3}},
	{ "prn", "1100",{-1},{0,1,2,3}},
	{ "jsr", "1101",{-1},{1,2,3}},
	{ "rts", "1110",{-1},{-1}},
	{ "stop", "1111",{-1},{-1}}
}; 



/* in the table: how many memory words the operand type requires */
mem_words_per_operand_type operands_vs_num_of_words_to_use[] = {
	{IMMEDIATE_OPERAND_TYPE, 1},
	{DIRECT_OPERAND_TYPE, 1},
	{MATRIX_OPERAND_TYPE, 2},
	{REGISTER_OPERAND_TYPE, 1}
};

registers registers_table[] = {
	{"r0", "0000"},
	{"r1", "0001"},
	{"r2", "0010"},
	{"r3", "0011"},
	{"r4", "0100"},
	{"r5", "0101"},
	{"r6", "0110"},
	{"r7", "0111"},
	{"r8", "1000"},
	{"r9", "1001"},
	{"r10", "1010"},
	{"r11", "1011"},
	{"r12", "1100"},
	{"r13", "1101"},
	{"r14", "1110"},
	{"r15", "1111"}
};
