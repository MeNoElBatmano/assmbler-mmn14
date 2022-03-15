#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include  "utility.h"
#include  "pre_process.h"
#include  "first_pass.h"
#include "data_structures.h"




int main(int argc, char *argv[]){

    
    FILE *fp;
    int arg_count = 1;
   
    

    for (arg_count; arg_count < argc; arg_count++)
    {
        
        char *file_name;
        file_name = create_file_name(argv[arg_count]);
        fp = fopen(file_name, "r");
        char c = 0;
        if(fp != NULL){
            pre_processor(fp, file_name);


        fclose(fp);
        }

        
        char *new_file_name;
        new_file_name = create_final_name(file_name);
        fp = fopen(new_file_name, "r");
        if(fp != NULL){
            first_pass(fp, file_name);


        fclose(fp);
        }
        
        
    }
    
    return 0;

}