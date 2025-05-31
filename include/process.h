#ifndef __PROCESS_H__
#define __PROCESS_H__
typedef struct process_info {
    int pid;      
    int status;    
} process_list;
extern process_list * processes;
extern int process_count;
extern int current_foreground_process_pid;
extern int current_status;
void initialize_process_list(int num_processes);
void free_process_list();
#endif