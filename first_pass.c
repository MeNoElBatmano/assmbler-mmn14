#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include  "first_pass.h"
#include "data_structures.h"


char *op_check(char *op_name);
opcodes opcodes_table[] = { 
	{ "mov","0", "0000000000000000",{0,1,2,3,},{1,2,3}},
	{ "cmp","0", "0000000000000001",{0,1,2,3},{0,1,2,3}},
	{ "add","1010", "0000000000000010",{0,1,2,3},{1,2,3}},
	{ "sub","1011", "0000000000000010",{0,1,2,3},{1,2,3,}},
	{ "lea","0", "0000000000001000",{-1},{1,2,3}},
	{ "clr","1010", "0000000000010000",{-1},{1,2,3}},
	{ "not","1011", "0000000000010000",{1,2},{1,2,3}},
	{ "inc","1100", "0000000000010000",{-1},{1,2,3}},
	{ "dec","1101", "0000000000010000",{-1},{1,2,3}},
	{ "jmp","1010", "0000000100000000",{-1},{1,2}},
	{ "bne","1011", "0000000100000000",{-1},{1,2}},
	{ "jsr","1100", "0000000100000000",{-1},{1,2,3}},
	{ "red","0", "0000100000000000",{-1},{0,1,2,3}},
	{ "prn","0", "0001000000000000",{-1},{1,2,3}},
	{ "rts","0", "0100000000000000",{-1},{-1}},
	{ "stop","0", "1000000000000000",{-1},{-1}}
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


int IC = 100;
int DC = 0;

int data_flag = 0;
int string_flag = 0;
int directive_flag = 0;
int extern_flag = 0;
int funct_flag = 0;
int immidiate_adressing_flag = 0;
int direct_adressing_flag = 0;
int index_adressing_flag = 0;
int register_adressing_flag = 0;
int L = 0;
int label_flag = 0;
int second_pass_flag = 0;


void first_pass(FILE *fp, char *file_name){
    label_table *table = malloc(sizeof(table));
    table->count = 0;
    table->head = NULL;
    label_node *node;
    int is_data = 0;
    int is_directive = 0;
    

    
    

    char *current_line = malloc(81 * sizeof(char));

     while(!feof(fp)) {
        fgets(current_line,81,fp);
        if (strstr(current_line, ":") != NULL)
        {   
            label_flag = 1;
            char *copy_line = malloc(81 * sizeof(char));
            strcpy(copy_line , current_line);
            handle_label(copy_line, node,table, is_data,IC);
        }
            
        if(strstr(current_line, ".data") != NULL || strstr(current_line, ".string") != NULL)
        {
                char *copy_line = malloc(81 * sizeof(char));
                strcpy(copy_line , current_line);
                if (strstr(current_line, ".data") != NULL)
                {
                    data_flag = 1;
                    line_to_tokens(copy_line, strstr(current_line, ".data") + 5, table);
                    data_flag = 0;
                    label_flag = 0;
                    continue;
                }
                else if(strstr(current_line, ".string") != NULL)\
                {
                    string_flag = 1;
                    line_to_tokens(copy_line, strstr(current_line, ".string") + 6, table);
                    string_flag = 0;
                    label_flag = 0;
                    continue;
                }
                free(copy_line);
                
        }
            
            
        
        else if(strstr(current_line, ".entry") != NULL)
        {
            label_flag = 0;
            continue;
        }
        else if(strstr(current_line, ".extern") != NULL)
        {
            extern_flag = 1;
            char *copy_line = malloc(81 * sizeof(char));
            strcpy(copy_line , current_line);
            handle_label(copy_line, node,table, is_data,IC);
            extern_flag = 0;
            label_flag = 0;
            continue;
        }
        /*DIRECTIVE*/
        else if(strlen(current_line) > 0)
        {

            directive_flag = 1;
            char *copy_line = malloc(81 * sizeof(char));
            strcpy(copy_line , current_line);
            if(label_flag)
            {
                line_to_tokens(copy_line, strstr(current_line, ":") + 1, table);
            }
            else
            {
                line_to_tokens(copy_line, current_line, table);
            }
            directive_flag = 0;
            L = 0;
            label_flag = 0;
            continue;
        }

        
    }
    label_node *tmp;
    tmp = table->head;
    while(tmp != NULL){
        printf("%s\n", tmp->name);
        printf("%d\n", tmp->base);
        printf("%d\n", tmp->offset);
        printf("%s\n", tmp->attr);
        tmp = tmp->next;
    }
    int icc = 100;
    while(icc != 140){
        printf("%s\n", code[icc]);
        icc++;
    }
    icc = 0;
    while(icc != 50){
        printf("%s\n",data[icc]);
        icc++;
    }
    label_flag = 0;
    free(current_line);
    second_pass(fp, file_name, table);
    
}





void second_pass(FILE *fp, char *file_name, label_table *table)
{
    char *current_line = malloc(81 * sizeof(char));
    second_pass_flag = 1;
    fseek(fp,0,SEEK_SET);

    while(!feof(fp))
    {
        fgets(current_line,81,fp);
        if (strstr(current_line, ".data") != NULL || strstr(current_line, ".string") != NULL || strstr(current_line, ".extern"))
        {
            continue;
        }
        else if(strstr(current_line, ".entry") != NULL)
        {
            char *label_name = calloc(81 , sizeof(char));
            label_name = find_label(strstr(current_line, ".entry") + 7);
            add_entry(label_name, table);
        }
        else
        {   
            if (strstr(current_line, ":") != NULL)
            {   
                label_flag = 1;
                line_to_tokens(current_line, strstr(current_line, ":") + 1, table);
            }
            else
            {
               line_to_tokens(current_line,current_line, table); 
            }
            
        }
        
    }
    second_pass_flag = 0;
    int count = 100;
    int count_d = 0;
    while (code[count][0] == '\0')
    {
        count++;
    }
    while(data[count_d][0] != '\0'){
        strcpy(code[count++], data[count_d++]);
    }

    print_ps();
    
     label_node *teemp;
    teemp = table->head;
    while(teemp != NULL){
        printf("%s\n", teemp->name);
        printf("%d\n", teemp->base);
        printf("%d\n", teemp->offset);
        printf("%s\n", teemp->attr);
        teemp = teemp->next;
    }
    int iccc = 100;
    while(iccc != 140){
        printf("%s\n", code[iccc]);
        iccc++;
    }
    iccc = 0;
    while(iccc != 50){
        printf("%s\n",data[iccc]);
        iccc++;
    }
}




void print_ps(){
    char *name = "ps.txt";
    FILE *new_fp;
    char *group = malloc(5);
    char *final_group = malloc(5);
    new_fp = fopen(name, "w");
    int count = 100;
    int hex_res;
    while(code[count][0] != '\0' && count != 100){

        if(strstr(code[count], "?"))
        {
            continue;
        }

        strncpy(group, code[count] , 4);
        hex_res = to_hex(group);
        sprintf(final_group,"%X",hex_res);
        fprintf(new_fp, "%s",final_group);
        fprintf(new_fp, "%s","-");

        strncpy(group, code[count] +4  , 4);
        hex_res = to_hex(group);
        sprintf(final_group,"%X",hex_res);
        fprintf(new_fp, "%s",final_group);
        fprintf(new_fp, "%s","-");

        strncpy(group, code[count] + 8 , 4);
        hex_res = to_hex(group);
        sprintf(final_group,"%X",hex_res);
        fprintf(new_fp, "%s",final_group);
        fprintf(new_fp, "%s","-");

        strncpy(group, code[count] + 12  , 4);
        hex_res = to_hex(group);
        sprintf(final_group,"%X",hex_res);
        fprintf(new_fp, "%s",final_group);
        fprintf(new_fp, "%s","-");

        strncpy(group, code[count] + 16 , 4);
        hex_res = to_hex(group);
        sprintf(final_group,"%X",hex_res);
        fprintf(new_fp, "%s",final_group);
        fprintf(new_fp, "%s","-");

        fprintf(new_fp, "%s","\n");


        count++;






    }
}
/* will find ':' occurance and save the label name that comes before it to the table, with base, offset , value and attributes*/
void handle_label(char* copy_line, label_node *node, label_table *table, int is_data, int IC){
    label_node *temp;
    char c;
    char *adress = copy_line;
    int i = 0;
    char label_name[81];
    int name_i = 0;
    /*Handling label if its extern command*/ 
    if(extern_flag){
        i+=7;

        while((c = copy_line[i]) != '\0')
        {
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '-' )
            {
            label_name[name_i++] = c;
                i++;
            }
            
            else {
                i++;
                continue;
            }
            
        }

        label_name[name_i] = 0;

        node = create_new_node(label_name, 0, 0,0, "external");
        add_node(node, table);
        


    }
    /*Handling label if its label definition in the current file*/ 
    else
    {
        while((c = copy_line[i]) != ':')
        {
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '-' )
            {
            label_name[name_i++] = c;
                i++;
            }
            
            else 
            {
                i++;
                continue;
            }
            
        }

        label_name[name_i] = 0;

        node = create_new_node(label_name, IC, 100 - IC%16 ,IC%16, "data");
        add_node(node, table);
          
        
    }
    

}



/*Function will recive a line and starting pointer and will procced to seperate the content to 3 tokens (max amount). If less then 3 tokens 
    it will hold NULL.
*/
void line_to_tokens(char *line , char *line_i, label_table *table ){
    char tokens[81][81] = {0}; 
    char *line_index = line_i;
    char c;
    char token[81];
    token[0] = '\0';
    int name_i = 0;
    int i = 0;


    

    if (second_pass_flag)
    {
        for (i = 0; i <3; i++)
        {
            while((c = *line_index) != '\0') 
            {
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '#' || c == '-' || c== '[' || c == ']')
            {
            token[name_i++] = c;
                line_index++;
            }
            
            else {
                if(strlen(token) > 0){
                    token[name_i] = '\0';
                    strcpy(tokens[i], token);
                    token[0] = '\0';
                    name_i = 0;
                    line_index++;
                    break;
                }
                line_index++;
                continue;
            }
            
            }
            
        }

        char *new_tok = malloc(81 * sizeof(char));
        strcpy(new_tok, tokens[1]);
        
        if (strstr(tokens[1], "#") == NULL && reg_check(new_tok) == 0)
        {
            handle_second_pass(tokens[1],  table);
        }
        strcpy(new_tok, tokens[2]);

        if (strstr(tokens[2], "#") == NULL && reg_check(new_tok) == 0)
        {
            handle_second_pass(tokens[1],  table);
        }

        return;

        
        
        
    }
    else{
        for (i = 0; i <3; i++)
        {
            while((c = *line_index) != '\0') 
            {
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '#' || c == '-' || c== '[' || c == ']')
            {
            token[name_i++] = c;
                line_index++;
            }
            
            else {
                if(strlen(token) > 0){
                    token[name_i] = '\0';
                    strcpy(tokens[i], token);
                    token[0] = '\0';
                    name_i = 0;
                    line_index++;
                    break;
                }
                line_index++;
                continue;
            }
            
            }
            
        }
    }
    

    /* after separating to tokens will handle the instruction/directive, translating them to binary and 
    addind to IC/DC*/
    if (directive_flag)
    {
        // extern opcodes_table;

        int op_number = 16;
        int op_i = 0;
        for (op_i ; op_i < op_number; op_i++)
        {   
            if(strcmp(tokens[0], opcodes_table[op_i].opcode) == 0)
            {
                if (strlen(opcodes_table[op_i].funct) == 4)
                {
                    funct_flag = 1;
                }
                
                handle_instruction(tokens, opcodes_table[op_i].opcode);
                funct_flag = 0;
                break;
            }
        }
    }

    else if(data_flag){
        to_binary(tokens, 0 , 0 , 0);
        data_flag = 0;
    }
    else if(string_flag){
        to_binary(tokens, 0 , 0 , 0);
        data_flag = 0;
    }
    
    
    
    
    

    




}






/*Will check the first token for valid opname and call function to handle binary code creation, push ot to code array and manage IC.
    Will check the second and third tokens (if not NULL) and increase the number of IC by the needed amount based on the adressing method of the token.
    After each valid token it will call binary builder function which will add a word to code array and increase the IC
*/
void handle_instruction(char tokens[81][81], char *op_name){
    int brk = 0;
    int i = 1;
    int iu;
    
    /*handle op code and func words and add to code arr*/
    if(tokens[0] != NULL){
        to_binary(tokens, 1 , op_name, 0);
        funct_flag = funct_flag_check(tokens[0]);
        if (tokens[1] != NULL)
        {
            strcpy(code[IC++], second_word(tokens, op_name));    
        }
        
    }




    
    for (i; i <= 2; i++)
    {
        if (tokens[i][0] == '\0')
        {
            return;
        }
        else if(reg_check(tokens[i]))
        {
            continue;
        }
        else if(strstr(tokens[i], "#") != NULL)
        {
            immidiate_adressing_flag  = 1;
            char new_tok[81] = {0};
            int i_im = 0;
            int i_new;
            for (i_im = 0; i_im < 80; i_im++)
            {
                if (tokens[i][i_im] != '#' && tokens[i][i_im] != '\0')
                {
                    new_tok[i_new++] = tokens[i][i_im];
                }
            }
            strcpy(tokens[i], new_tok);
            
            strcpy(code[IC++], convert_to_bin_immidiate(atoi(tokens[i])));
            immidiate_adressing_flag  = 0;

        }
        else if(strstr(tokens[i], "[") != NULL && strstr(tokens[i], "]") == NULL){
            index_adressing_flag  = 1;
            L+=2;
            strcpy(code[IC], "???");
            IC++;
            strcpy(code[IC], "???");
            IC++;
            index_adressing_flag  = 0;
        }
        
        else{
           direct_adressing_flag  = 1; 
           L+=2;
           strcpy(code[IC], "???");
            IC++;
            strcpy(code[IC], "???");
            IC++;
            L++;
           direct_adressing_flag  = 0; 
        }
    }
    funct_flag = 0;
}




 char *second_word(char tokens[81][81], char *op_name){
    char *bnr_final;
    bnr_final = calloc(20, sizeof(char));
    strcat(bnr_final, "0");

    strcat(bnr_final, "100");

    if(funct_flag){
            strcat(bnr_final, funct_check(op_name));

        }
    else
        {
            strcat(bnr_final, "0000");
        }
    
    strcat(bnr_final, source_check(tokens[1]));
        strcat(bnr_final, source_adress_check(tokens[1]));
        strcat(bnr_final, source_check(tokens[2]));
        strcat(bnr_final, source_adress_check(tokens[2]));


    return bnr_final;
 }






void to_binary(char tokens[81][81], int is_op, char *op_name, int dir_i){
    char bnr_final[20];
    bnr_final[0] = '\0';
    strcat(bnr_final, "0");
    strcat(bnr_final, "100");


    
    
    if(directive_flag)
    {
        
        if (is_op)
        {
            strcat(bnr_final, op_check(op_name));
            strcat(code[IC], bnr_final);
            IC++;
            L++;
            return;

        }
        /* ARE can onle be A at this stage*/
        

        if(funct_flag){
            strcat(bnr_final, funct_check(op_name));

        }
        else
        {
            strcat(bnr_final, "0000");
        }

        if(immidiate_adressing_flag){
            strcat(bnr_final, convert_to_bin(atoi(tokens[dir_i])));
            strcpy(code[IC], bnr_final);
            IC++;
            return;

        }


        if(L == 2)
        {
        strcat(bnr_final, source_check(tokens[dir_i]));
        strcat(bnr_final, source_adress_check(tokens[dir_i]));
        strcat(bnr_final, source_check(tokens[dir_i + 1]));
        strcat(bnr_final, source_adress_check(tokens[dir_i + 1]));
        }
        
        strcpy(code[IC], bnr_final);
        IC++;
        
        
       
    }
    
    if (data_flag)
    {
        int i = 0;
        while (*tokens[i] != '\0')
        {
            strcpy(bnr_final, convert_to_bin(atoi(tokens[i])));
            strcpy(data[DC++], bnr_final);
            i++;
            return;
        }
        
        
    }
    else if(string_flag){
        int i = 0, j = 0;
        while (*tokens[i] != '\0')
        {
            while (tokens[i][j] != '\0')
            {
                strcpy(bnr_final, convert_to_bin((int)tokens[i][j]));
                strcpy(data[DC++], bnr_final);
                j++;
            }
            i++;
        }
        
    }
    



}


char* convert_to_bin(int n){
    char *res;
    res = (char *)calloc(21, sizeof(char));
    int  c, k;
    // if (data_flag)
    // {
    //     n = atoi(token);
    // }
    // else if(string_flag){
    //     n = (int)*token;
    // }
    

    for (c = 19; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
        strcat(res, "1");
        else
        strcat(res, "0");
    }
    return res;
    
}



char* convert_to_bin_immidiate(int n){
    char *res;
    res = (char *)calloc(21, sizeof(char));
    int  c, k;
    strcat(res, "0100");
    // if (data_flag)
    // {
    //     n = atoi(token);
    // }
    // else if(string_flag){
    //     n = (int)*token;
    // }
    

    for (c = 15; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
        strcat(res, "1");
        else
        strcat(res, "0");
    }
    return res;
    
}

char* convert_to_bin_extern(int n){
    char *res;
    res = (char *)calloc(21, sizeof(char));
    int  c, k;
    strcat(res, "0001");
    // if (data_flag)
    // {
    //     n = atoi(token);
    // }
    // else if(string_flag){
    //     n = (int)*token;
    // }
    

    for (c = 15; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
        strcat(res, "1");
        else
        strcat(res, "0");
    }
    return res;
    
}

char* convert_to_bin_direct(int n){
    char *res;
    res = (char *)calloc(21, sizeof(char));
    int  c, k;
    strcat(res, "0010");
    // if (data_flag)
    // {
    //     n = atoi(token);
    // }
    // else if(string_flag){
    //     n = (int)*token;
    // }
    

    for (c = 15; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
        strcat(res, "1");
        else
        strcat(res, "0");
    }
    return res;
    
}

char *op_check(char *op_name){
    int i = 0;
    int op_number = 16;

    for (i ; i < op_number; i++)
        {   
            if(strcmp(op_name, opcodes_table[i].opcode) == 0)
            {
                return opcodes_table[i].binary_val;
            };
        }
}


char *funct_check(char *op_name){
    int i = 0;
    for ( i = 0; i < 16; i++)
    {
        if(strcmp(opcodes_table[i].opcode, op_name) == 0)
        {
            return opcodes_table[i].funct;
        }
        
    }
    
}

char *source_check(char *token){
    if(register_adressing_flag)
    {
        int i = 0;
        for ( i = 0; i < 16; i++)
        {
            if(strcmp(registers_table[i].register_name, token) == 0)
            {
                return registers_table[i].reg_binary;
            }
            
        }
    }
    else return "0000";

}


int reg_check(char *token){
        int i = 0;
        for ( i = 0; i < 16; i++)
        {
            if(strstr(registers_table[i].register_name, token) != NULL)
            {
                return 1;
            }
        }
        return 0;
       
}
    




char *source_adress_check(char *token){
    if (strstr(token, "#") != NULL)
    {
        return "00";
    }
    else if(strstr(token, "[") != NULL && strstr(token, "]") == NULL)
    {
        return "10";
    }
    else if(reg_check(token))
    {
        return "11";
    }
    else
    {
        return "01";
    }
    
}

// char *dest_check(){
    
// }

// char *dest_adress_check(){
    
// }

label_node *create_new_node(char *name,int value, int base, int offset, char *attr){
    label_node *result = malloc(sizeof(label_node));

    if(result == NULL){
            printf("error!!!");
            exit(1);
        }

    result->value = value;
    result->base = base;
    result->offset = offset;
    result->name = (char *)malloc(strlen(name) * sizeof(char) + 1);
    result->attr = (char *)calloc(81, sizeof(char));
    strcpy(result->attr, attr);
    strcpy(result->name, name);
    result->next = NULL;
    return result;  


}

void add_node(label_node *node, label_table *table){
    label_node *temp;
    if (table->head == NULL)
            {
                table->head = node;
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
    return;
}

int funct_flag_check(char* token){
    int i = 0;
    for ( i = 0; i < 16; i++)
    {
        if(strcmp(opcodes_table[i].opcode, token) == 0)
        {
            if(strlen(opcodes_table[i].funct) > 2)
                return 1;
        }
        
    }
    return 0;
}

int to_hex(char *word_group){
    char *bin=word_group;
    char *a = bin;
    int num = 0;
    do {
        int b = *a=='1'?1:0;
        num = (num<<1)|b;
        a++;
    } while (*a);
    return num;
    
}

void handle_second_pass(char *token,label_table *table ){
    int i_counter = 100;
    char *res_base = malloc(81 * sizeof(char));
    char *res_offset = malloc(81 * sizeof(char));
    int res_b,res_o;
    int is_extern = 0;

  
        while(strstr(code[i_counter], "?") == NULL){
            i_counter++;
        }

        label_node *tmp;
        tmp = table->head;
        while(tmp != NULL){
            if(strstr(token, tmp->name)){
                char *a = malloc(81);
                char *b = malloc(81);
                sprintf(a,"%d",tmp->base);
                sprintf(b,"%d",tmp->offset);
                strcpy(res_base, a);
                strcpy(res_offset, b);

                if(strstr(tmp->attr, "external")){
                is_extern = 1;

                res_b = atoi(res_base);
                res_o = atoi(res_offset);
                }

                if(is_extern)
                {
                    strcpy(code[i_counter], convert_to_bin_extern(res_b) );
                strcpy(code[i_counter +1 ], convert_to_bin_extern(res_o) );
                }
                else{
                    strcpy(code[i_counter], convert_to_bin_direct(res_b) );
                    strcpy(code[i_counter +1 ], convert_to_bin_direct(res_o) );
                }
            
            }
            tmp = tmp->next;
            

        }
        return;
        
        

    
    

}











char* find_label(char* address){
    char c;
    char *adress = address;
    int i = 0;
    char *label_name = calloc(81 , sizeof(char));
    int name_i = 0;


         while((c = adress[i]) != '\0')
        {
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
            {
            label_name[name_i++] = c;
                i++;
            }
            
            else {
                i++;
                continue;
            }
            
        }

        label_name[name_i] = 0;
        return label_name;
}

void add_entry(char* label, label_table *table){
    label_node *temp;
    temp = table->head;
    while (temp != NULL)
    {
        if(strcmp(temp->name, label) == 0){
            strcat(temp->attr, ",entry");
            return;
        }
        temp = temp->next;
    }
    
}