#ifndef __PIPE_C__
#define __PIPE_C__

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "custom.h"
#include<wait.h>
#include "IO.h"
int check_io(const char* command_local) {
    char *command = strdup(command_local);
    char *saveptr;
    char *token = strtok_r(command, " ", &saveptr);
    while(token != NULL) {
        if(strcmp(token, ">") == 0 || strcmp(token, "<") == 0 || strcmp(token, ">>") == 0) {
            free(command);
            return 1;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }
    free(command);
    return 0;
}
void pipe_handling(int no_of_pipes, char* command, char* home,int pid) {
    char* token = strtok(command, "|");
    char* pair_of_commands[no_of_pipes + 1];
    int i = 0;
    while (token != NULL) {
        pair_of_commands[i++] = token;
        token = strtok(NULL, "|");
    }
    pair_of_commands[i] = NULL;
    int n=i;
    //printf("%d\n",n);
    if (n != no_of_pipes + 1 || n < 2) {
        printf("Invalid use of pipe\n");
        // printf("%d\n", n);
        return;
    }
    int pipes[2];
    int prev_output = 0;
    for (int k = 0; k < n; k++) {
        if(pipe(pipes) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        int child = fork();
        if(child<0){
            printf("Child cannot be created\n");
            exit(EXIT_FAILURE);
        }
        else if(child==0){
            dup2(prev_output, 0); 
            if(k != n -1) {
                dup2(pipes[1], 1); 
            }
            close(pipes[0]);
            if(check_io(pair_of_commands[k])) {
                redirect(pair_of_commands[k],home,pid);
            }
            else {
                run_command(pair_of_commands[k], home,pid);
            }
            exit(0);
        }
        else{
            wait(NULL);
            close(pipes[1]);
            prev_output = pipes[0]; 
        }
    }
    return;
}

#endif
