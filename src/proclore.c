#ifndef __PROCLORE_C__
#define __PROCLORE_C__
#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "display.h"
// #include "background.h"
#define PATH_MAX 4096
void proclore(int pid,const char* home) {
    char path[PATH_MAX], buffer[4096], *token;
    int fd;
    pid_t pgrp;
    unsigned long vmem;
    char status[4096];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open stat file");
        return;
    }
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Failed to read stat file");
        close(fd);
        return;
    }
    buffer[bytesRead] = '\0'; 
    close(fd);
    char* saveptr;
    token = strtok_r(buffer, " ",&saveptr);
    int field = 0;
    int tpgid ;
    while (token != NULL) {
        switch (field) {
            case 2: 
                // Status
                strcpy(status, token);
                break;
            case 4: 
                // Process Group ID
                pgrp = atoi(token);
                break;
            case 7:
                //foreground process groud id
                tpgid = atoi(token);
                break;
            case 22: 
                // Virtual Memory Size
                vmem = strtoul(token, NULL, 10);
                break;
        }
        token[strcspn(token, "\n")] = '\0';
        token = strtok_r(NULL, " ",&saveptr);
        field++;
    }
    char status_str[2];
    if (pgrp == tpgid ) {
        strcat(status, "+");
    }
    printf("pid : %d\n", pid);
    printf("process status : %s\n", status);
    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %lu\n", vmem);
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    char exe_path[PATH_MAX];
    ssize_t len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("Failed to read exe link");
        return;
    }
    exe_path[len] = '\0'; 
    replaceSubstring(exe_path, home, "~");
    printf("Executable path : %s\n", exe_path);
    return;
}

#endif