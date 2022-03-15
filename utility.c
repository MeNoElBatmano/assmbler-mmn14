
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Creates new string with .as extension from argv file name*/ 
char *create_file_name(char *name){
    char *mod_file_name = malloc(strlen(name) + 4);
    strcpy(mod_file_name, name);
    strcat(mod_file_name, ".txt");

    return mod_file_name;
}