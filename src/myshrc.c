#ifndef __MYSHRC_C__
#define __MYSHRC_C__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include "display.h"
#include "myshrc.h"
#include "custom.h"
#include<ctype.h>
void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)  
        return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
}
int search(char* home,const char* command_local,int pid){
    char *command = strdup(command_local);
    char *saveptr1;
    char *token1 = strtok_r(command, " ", &saveptr1);
    char path[4096];  
    sprintf(path, "%s/.myshrc", home);
    // printf("%s\n",path);
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("NO MYSHRC FILE\n");
        return 0;
    }
    char* string = (char*)malloc(sizeof(char) * 4096);
    char *saveptr;
    while (fgets(string, 4096, fptr) != NULL) {
        if (strncmp(string, "alias", 5) == 0) {
            char *alias_name = strtok_r(string + 6, "=", &saveptr); 
            trim_whitespace(alias_name);
            if (alias_name) {
                char *alias_command = strtok_r(NULL, "\n", &saveptr);
                if (alias_command) {
                    // printf("%s-%s\n",token1, alias_name);
                    if (strcmp(alias_name, token1) == 0) {
                        run_command(alias_command, home, pid); 
                        fclose(fptr);
                        free(string);
                        return 1;
                    }
                }
            }
        }
    }
    // printf("NO ALIAS FOUND\n");
    free(string);      
    fclose(fptr);
    return 0;
}


#endif