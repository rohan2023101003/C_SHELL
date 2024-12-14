#ifndef __BACKGROUND_C__
#define __BACKGROUND_C__
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process.h"
// #include <signal.h>

void background_execute(char* args[]) {
    int bgp = fork();
    setpgid(0,0);      
    if (bgp < 0) {
        perror("fork error");
        current_status = 1;
        return;
    }
    else if (bgp == 0) {
        printf("%d\n", getpid());
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
            exit(1);
        }
        exit(0);
    } else {
        processes[process_count].pid = bgp;
        processes[process_count].status = 0;
        process_count++;
        return;
    }
}
void make_agrs(char* command) {
    char *saveptr;
    char *token = strtok_r(command, " ", &saveptr);
    if (token == NULL) {
        printf("Invalid Command\n");
        return;
    }
    char *args[4096];
    int i = 0;

    while (token != NULL) {
        // printf("%s\n", token);
        token[strcspn(token, "\n")] = '\0'; 
        if (strcmp(token, "&") == 0) {
            break;  
        }
        args[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }
    args[i] = NULL;
    background_execute(args);
    return;
}

#endif
