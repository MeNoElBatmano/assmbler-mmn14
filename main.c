#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include  "utility.c"
#include  "pre_process.c"


int main(int argc, char *argv[]){

    // char *file_names[argc - 1];
    FILE *fp;
    int arg_count = 1;
   
    

    for (arg_count; arg_count < argc; arg_count++)
    {
        
        char *file_name;
        file_name = create_file_name(argv[arg_count]);
        fp = fopen(file_name, "r");
        char c = 0;
        if(fp != NULL){
            pre_proccesor(fp);
        }
        // while(1){
        //     c = fgetc(fp);
        //     if (c == EOF)
        //     {
        //         break;
        //     }
        //     printf("%c",c);
            
            
        // }
        // fclose(fp);
    }
    


}