#ifndef __FG_C__
#define __FG_C__
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <errno.h>
// #include "background.h" 
// #include "display.h"
// #include<string.h>
#include<errno.h>

void fg(int pid) {
    useconds_t t = 1000;
    usleep(t);
    int flag=0;
    int i=0;
    for(i=0;i<process_count;i++){
        if(processes[i].pid==pid && processes[i].status==0){
            flag=1;
            break;
        }
    }
    // if(flag==0){
    //     printf("No such process\n");
    //     current_status=0;
    //     return;
    // }
    int shellpid = getpgid(0);
    signal(SIGTTOU, SIG_IGN);
    if (kill(pid, SIGCONT) == -1) {
        if (errno == ESRCH) {
            printf("No such process found\n");
        } else {
            perror("Error sending signal");
        }
        return;
    }
    tcsetpgrp(STDIN_FILENO, getpgid(pid));
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
        perror("Failed to wait for process");
        return;
    }
    tcsetpgrp(0,shellpid);
    signal(SIGTTOU, SIG_DFL);
    if(WIFSTOPPED(status))
        {
        printf("[%d] Suspended\n",pid);
    }
    if(WIFEXITED(status))
    {
        processes[i].status = 1;
    }
    return;
}


void execute_fg_command(int pid) {
    if (pid > 0) {
        fg(pid);
    } else {
        printf("Invalid process ID\n");
    }
    return;
}

#endif