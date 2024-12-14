#ifndef __ACTIVITIES_C__
#define __ACTIVITIES_C__

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include "activities.h"

const char* get_state(char state_char) {
    switch (state_char) {
        case 'R': return "Running";
        case 'S': return "Running";
        case 'T': return "Stopped";
        case 'Z': return "Zombie";
        default: return "Unknown";
    }
}

int compare_by_pid(const void *a, const void *b) {
    process_details *procA = (process_details *)a;
    process_details *procB = (process_details *)b;
    return procA->pid - procB->pid;
}

process_details* get_processes(char* home, int pid,int* process_count) {
    // printf("%d\n", pid);
    struct dirent **items;
    int n = scandir("/proc", &items, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
        return NULL;
    }

    process_details* processes=(process_details*)malloc(sizeof(process_details)*1024);  // Assuming a max of 1024 processes
   
    char buffer[4096];
    char path[4096];

    for (int i = 0; i < n; i++) {
        // printf("%s\n",items[i]->d_name);
        if (isdigit(items[i]->d_name[0])) {
            // printf("%s\n",items[i]->d_name);
            snprintf(path, sizeof(path), "/proc/%s/stat", items[i]->d_name);
            // printf("%s\n",path);
            int fd = open(path, O_RDONLY);
            if (fd < 0) {
                perror("Failed to open stat file");
                continue;  
            }

            int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
            if (bytesRead == -1) {
                perror("Failed to read stat file");
                close(fd);
                continue; 
            }
            buffer[bytesRead] = '\0'; 
            close(fd);


            char* saveptr;
            char* token = strtok_r(buffer, " ", &saveptr);
            int field = 0;

            process_details proc = {0};  
            while (token != NULL) {
                switch (field) {
                    case 0: 
                        proc.pid = atoi(token);  // Process PID
                        break;
                    case 1: 
                        token++;
                        token[strlen(token) - 1] = '\0'; 
                        strcpy(proc.command, token);
                        break;
                    case 2:
                        proc.state = token[0];  // Process state
                        break;
                    case 3:
                        proc.ppid = atoi(token);  // Parent PID
                        break;
                }
                token = strtok_r(NULL, " ", &saveptr);
                field++;
            }
            if (proc.ppid == pid) {
                processes[*process_count] = proc;
                *process_count += 1;
                // printf("pid : %d\n", proc.pid);
            }
        }
        free(items[i]);
    }
    free(items);

    
    qsort(processes, *process_count, sizeof(process_details), compare_by_pid);

    // // Print the processes
    // for (int i = 0; i < process_count; i++) {
        
    //     printf("%d : %s - %s\n", processes[i].pid, processes[i].command, get_state(processes[i].state));
    //     // if(processes[i].state=='R'){
    //     //     exit(0);
    //     // }
    // }
    return processes;
}
void activity(char* home, int pid){
    int process_count=0;
    process_details* processes = get_processes(home, pid,&process_count);
    for (int i = 0; i < process_count; i++) {
        printf("%d : %s - %s\n", processes[i].pid, processes[i].command, get_state(processes[i].state));
    }
    free(processes);
}
#endif
