assembler: main.c pre_process.c first_pass.c
	gcc -g -Wall -pedantic main.c pre_process.c utility.c first_pass.c -o main

