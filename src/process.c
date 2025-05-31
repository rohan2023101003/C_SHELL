#ifndef __PROCESS_C__
#define __PROCESS_C__

#include "process.h"
#include <stdlib.h>
#include <stdio.h>
process_list* processes = NULL;
int process_count = 0;
int current_foreground_process_pid = -1;
int current_status = 1;

void initialize_process_list(int num_processes) {
    processes = (process_list*)malloc(num_processes * sizeof(process_list));
    if (processes == NULL) {
        perror("Failed to allocate memory for process list");
        exit(1);
    }
}
void free_process_list() {
    if (processes != NULL) {
        free(processes);
        processes = NULL;
        process_count = 0;
    }
}
#endif