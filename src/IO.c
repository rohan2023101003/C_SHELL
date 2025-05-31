#ifndef __IO_C__
#define __IO_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
// #include "foreground.h"
#include "custom.h"
char* args_to_string(char* args[], int num_args) {
    int total_length = 0;
    for (int i = 0; i < num_args; i++) {
        total_length += strlen(args[i]) + 1; 
    }
    char* result = malloc(total_length);
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
    for (int i = 0; i < num_args; i++) {
        strcat(result, args[i]);
        if (i < num_args - 1) {
            strcat(result, " ");
        }
    }
    return result;
}
void redirect(char *command_local, char* home,int pid){
    char *command = strdup(command_local);
    char *saveptr;
    char *token = strtok_r(command, " ", &saveptr);
    int overwrite = 0, append = 0, read = 0;
    char *overwrite_file = NULL;
    char *append_file = NULL;
    char *read_file = NULL;
    char *args[4096];
    int i = 0;

    memset(args, 0, sizeof(args));

    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            overwrite = 1;
            token = strtok_r(NULL, " ", &saveptr);
            overwrite_file = token;
        }
        else if (strcmp(token, ">>") == 0) {
            append = 1;
            token = strtok_r(NULL, " ", &saveptr);
            append_file = token;
        }
        else if (strcmp(token, "<") == 0) {
            read = 1;
            token = strtok_r(NULL, " ", &saveptr);
            read_file = token;
        }
        else {
            args[i++] = token;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }
    args[i] = NULL;
    if(overwrite_file == NULL && append_file == NULL && read_file == NULL) {
        printf("Invalid use of redirection\n");
        return;
    }
     if (overwrite_file != NULL && overwrite_file[0] == '~') {
        char* temp = (char*)malloc(sizeof(char) * 4096);
        if (temp == NULL) {
            perror("malloc");
            free(command);
            return;
        }
        strcpy(temp, home);
        strcat(temp, overwrite_file + 1);
        overwrite_file = temp;
    }

    if (append_file != NULL && append_file[0] == '~') {
        char* temp = (char*)malloc(sizeof(char) * 4096);
        if (temp == NULL) {
            perror("malloc");
            free(command);
            return;
        }
        strcpy(temp, home);
        strcat(temp, append_file + 1);
        append_file = temp;
    }

    if (read_file != NULL && read_file[0] == '~') {
        char* temp = (char*)malloc(sizeof(char) * 4096);
        if (temp == NULL) {
            perror("malloc");
            free(command);
            return;
        }
        strcpy(temp, home);
        strcat(temp, read_file + 1);
        read_file = temp;
    }
    int saved_stdout = dup(1);
    int saved_stdin = dup(0);
    if (saved_stdout < 0 || saved_stdin < 0) {
        perror("Failed to save file descriptors");
        free(command);
        return;
    }
    if (overwrite) {
        int fd_overwrite = open(overwrite_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_overwrite < 0) {
            perror("Error opening file for overwrite");
            free(command);
            close(saved_stdout);
            close(saved_stdin);
            return;
        }
        dup2(fd_overwrite, 1);
        close(fd_overwrite);
    } else if (append) {
        int fd_append = open(append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_append < 0) {
            perror("Error opening file for append");
            free(command);
            close(saved_stdout);
            close(saved_stdin);
            return;
        }
        dup2(fd_append, 1);
        close(fd_append);
    }
    if (read) {
        int fd_read = open(read_file, O_RDONLY);
        if (fd_read < 0) {
            perror("No such input file found!");
            free(command);
            close(saved_stdout);
            close(saved_stdin);
            return;
        }
        dup2(fd_read, 0);
        close(fd_read);
    }
    int self_made_commands =0;
    for(int j=0;j<i;j++){
         if(strcmp(args[j],"hop")==0 || strcmp(args[j],"reveal")==0 || strcmp(args[j],"proclore")==0 || strcmp(args[j],"seek")==0 || strcmp(args[j],"ping")==0 || strcmp(args[j],"log execute")==0  ||strcmp(args[j],"bg")==0 || strcmp(args[j],"fg")==0 || strcmp(args[j],"iMan")==0 || strcmp("neonate",args[j])==0 || strcmp("activities",args[j])==0){
             self_made_commands = 1;
             break;
         }
    }
    if(self_made_commands && read){
        char input[4096];
        if(fgets(input, sizeof(input), stdin) == NULL) {
            strcpy(input," ");
        }
        args[i]=input;
        args[i+1]=NULL;
        run_command(args_to_string(args, i+1), home,pid);
    }
    else{
        run_command(args_to_string(args, i), home,pid);
    }
    
    fflush(stdout);
    clearerr(stdin);
    if (dup2(saved_stdout, 1) < 0) {
        perror("Failed to restore STDOUT");
    }
    if (dup2(saved_stdin, 0) < 0) {
        perror("Failed to restore STDIN");
    }
    close(saved_stdout);
    close(saved_stdin);

    memset(args, 0, sizeof(args));
    if (overwrite_file != NULL && overwrite_file[0] == '/') free(overwrite_file);
    if (append_file != NULL && append_file[0] == '/') free(append_file);
    if (read_file != NULL && read_file[0] == '/') free(read_file);
    free(command); 
    return;
}

#endif
