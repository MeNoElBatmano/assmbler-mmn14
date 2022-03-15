#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include  "pre_process.h"



void pre_processor(FILE *fp, char *file_name){
    macro_table *table = malloc(sizeof(table));
    table->count = 0;
    table->head = NULL;
    macro_node *node;
    handle_macors(fp, node, table);
    print_new_file(fp, table, file_name);
    

    free(table);
}



void handle_macors(FILE *fp,  macro_node *node,  macro_table *table ){
    char *current_line = malloc(81 * sizeof(char));
    int file_i = 0;
    char *macro_i;
    char *sub_string = "macro";


    while(!feof(fp)) {
        fgets(current_line,81,fp);
        if (strstr(current_line, sub_string) != NULL)
        {
            macro_i =(strstr(current_line, sub_string)) + 5;
            save_macro(macro_i, fp, current_line, node, table);
        }

        
    }
    
}


void save_macro(char *macro_i,FILE *fp, char *current_line, macro_node *node,  macro_table *table ){
    macro_node *temp;
    char *adress = macro_i;
    char macro_name[81];
    char *macro_data = calloc(81, sizeof(char));
    char c;
    int name_i = 0;
    int macro_end = 0;
    int line_length = 0;
    int data_i = 1;

    while((c = *adress) != 0)
    {
        if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' )
        {
           macro_name[name_i++] = c;
            adress++;
        }
        
        else {
            adress++;
            continue;
        }
        
    }
    macro_name[name_i] = 0;
    
    
    

    
    
    while (macro_end != 1)
    {
        fgets(current_line,81,fp);
        if (strstr(current_line, "endm") == NULL)
        {
            data_i++;
            line_length += strlen(current_line) + 1;
            macro_data = realloc(macro_data , (line_length * sizeof(char)));
            if (macro_data == NULL)
            {
                printf("ERROR!");
                exit(1);
            }
            
            strcat(macro_data, current_line);
        }
        else (macro_end = 1);
    }


    node = malloc(sizeof(node));   
    node->name = malloc(strlen(macro_name));
    node->data = malloc(strlen(macro_data) + 1);
    strcpy(node->name, macro_name);
    strcpy(node->data, macro_data);
    node->next=NULL;

    if (table->head == NULL)
    {
        table->head = node;
        node->next = NULL;
    }
    else
    {
    temp = table->head;
    while ((temp->next) != NULL)
    {
        temp = temp->next;
    }
    temp->next=node;
    }
    
    
    free(macro_data);
    free(current_line);
    
}


void print_new_file(FILE *fp,macro_table *table,char *file_name){
    char *new_name;
    char *add = "final_";
    new_name = malloc(strlen(file_name) + strlen(add) + 1);
    strcpy(new_name, add);
    strcat(new_name, file_name);
    FILE *new_fp;
    fseek(fp, 0 , SEEK_SET);
    new_fp = fopen(new_name, "w");
    

    char *current_line = malloc(81);
    macro_node *temporary = malloc(sizeof(temporary));



    while(!feof(fp)) {
        int in_macro = 0;
        fgets(current_line,81,fp);
        temporary = table->head;

        if(strstr(current_line, "macro") != NULL)
        {
            while (temporary != NULL)
            {
                if(strstr(current_line, temporary->name) != NULL)
                {
                    fprintf(new_fp, "%s",temporary->data);
                    break;
                }
                temporary = temporary->next;
            }

            while(strstr(current_line, "endm") == NULL)
            {
                fgets(current_line,81,fp);
            }
        }

        else if(strstr(current_line, "macro") == NULL && strstr(current_line, "endm") == NULL)
        {
            while (temporary != NULL)
            {
                if(strstr(current_line, temporary->name) != NULL)
                {
                    fprintf(new_fp, "%s",temporary->data);
                    in_macro = 1;
                    break;
                }
                temporary = temporary->next;
            }
            if (in_macro == 0){
                fprintf(new_fp, "%s",current_line);
            }
        }

    

        




       /* if (in_macro == 1 && strstr(current_line, "endm") == NULL)
        {
            continue;
        }
        else if(in_macro == 1 && strstr(current_line, "endm") != NULL)
        {
            in_macro = 0;
            continue;
        }
        else
        {
            while (temporary != NULL)
            {
                if(strstr(current_line, temporary->name) != NULL)
                {
                    fprintf(new_fp, "%s",temporary->data);
                    in_macro = 1;
                    break;
                }
                temporary = temporary->next;
            }
            if (in_macro == 0)
            {
                fprintf(new_fp, "%s",current_line);

            }
        }*/

        

        
       /* while (temporary->next != NULL )
        {
            while (in_macro == 1)
            {
                fgets(current_line,81,fp);
                if (strstr(current_line, "endm") != NULL)
                {
                    in_macro = 0;
                    continue;
                }
                
            }
            
            
            if (strstr(current_line, temporary->name) != NULL)
            {
                fprintf(new_fp, "%s",temporary->data);
                in_macro = 1;
            }

            else{
                fprintf(new_fp, "%s",current_line);
               
            }

        temporary = temporary->next;

        }    */
    }
    fclose(new_fp);

}