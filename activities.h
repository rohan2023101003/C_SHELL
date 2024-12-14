#ifndef __ACTIVITIES_H__
#define __ACTIVITIES_H__
void activity(char* home,int pid);
const char* get_state(char state_char);
int compare_by_pid(const void *a, const void *b);
typedef struct {
    int pid;
    int ppid;
    char command[4096];
    char state;
} process_details;
process_details* get_processes(char* home, int pid,int* process_count);
#endif