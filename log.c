#ifndef __LOG_C__
#define __LOG_C__
#include<stdio.h>
#include "log.h"
#include<string.h>
#include<stdlib.h>
#define MAX_STRINGS 15
#define MAX_LENGTH 4096
char last_command[MAX_STRINGS];
void log_purge(char* path){
    remove(path);
    return;
}
void log_print(char* path){
    FILE *fptr;
    // printf("%s\n",path);
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("NO HISTORY\n");
        return;
    }
    char ch;
    while ((ch = fgetc(fptr)) != EOF) {
        printf("%c", ch);
    }
    fclose(fptr);
    return;
}
int count_commands(char* path)
{
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        // printf("Error!");
        return 0;
    }
     int count = 0;
    char ch;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fclose(fptr);
    return count;
}
char* get_command(char* path,int t){
    int n = count_commands(path);
    t = n-t+1;
    if(t<=0){
        printf("NO SUCH COMMAND\n");
        return NULL;
    }
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("NO HISTORY\n");
        return NULL;
    }
    char* command = (char*)malloc(MAX_LENGTH*sizeof(char));
    char ch;
    int count=1;
     while (count!=t  && (ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    int i = 0;
    while((ch = fgetc(fptr)) != '\n'){
        command[i] = ch;
        i++;
    }
    command[i] = '\0';
    // printf("%s\n",command);
    fclose(fptr);
    return command;
}
int check_log(char* command){
    char temp[4096];
    strcpy(temp,command);
    char* token=strtok(temp," ");
     while(token!=NULL){
        token[strcspn(token, "\n")] = '\0';
        if(strcmp(token,"log")==0){
            return 1;
        }
        token=strtok(NULL," ");
    }
    return 0;
}
void skip_first_line(char* home)
{
    char path[4096];  
    sprintf(path, "%s/log.txt", home);
    FILE* fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("file not opened : Error!");
        return;
    }
    char ch;
    while((ch = fgetc(fptr)) != '\n'){
        continue;
    }
    char path2[4096];  
     sprintf(path2, "%s/temp.txt", home);

    FILE* fptr2 = fopen(path2, "w");
    if (fptr2 == NULL)
    {
        printf("file not open while skip 2:Error!");
        return;
    }
    while((ch = fgetc(fptr)) != EOF){
        fprintf(fptr2, "%c", ch);
    }
    fclose(fptr);
    fclose(fptr2);
    remove(path);
    rename(path2, path);
    return;
}

void log_command(char *command,char* home)
{
    char path[4096];  
    sprintf(path, "%s/log.txt", home);
    if(strcmp(command,last_command)==0){
        return;
    }
    if(check_log(command)){
        return;
    }
    if(count_commands(path)>=MAX_STRINGS){
        skip_first_line(home);
    }
    FILE *fptr;
    fptr = fopen(path, "a");
    if (fptr == NULL)
    {
        printf("file not opened:Error!");
        return;
    }
    fprintf(fptr, "%s", command);
    strcpy(last_command,command);
    fclose(fptr);
    return;
}

#endif