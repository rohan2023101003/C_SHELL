#ifndef __SIG_C__
#define __SIG_C__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
// #include <string.h>
#include"sig.h"
#include "activities.h"
#include "display.h"
#include "process.h"
#include "wait.h"
// #include "background.h"
void handle_background_termination(int signal){
    int pid;
    int status1;
    if ((pid = waitpid(-1, &status1, WNOHANG)) != -1){
        if(pid<=0){
            return;
        }
        for (int i = 0; i < process_count; i++){
            if (processes[i].pid == pid)
            {
                printf("Process  ID [%d] exited with exit code %d\n", processes[i].pid, status1);
                processes[i].status = 1; // Mark process as finished
            }
        }
    }
    fflush(stdout);
}
void handle_sigquit(){
    // Log out of the shell
    int processs_cnt=0;
    process_details* proces = get_processes(home_dir,shell_pid,&processs_cnt);
    for(int i=0;i<processs_cnt;i++){
            kill(proces[i].pid,SIGKILL);
            printf("%d : Process Killed \n",proces[i].pid);
    }
    free(proces);
    printf("Logging out...\n");
    printf("Bye!\n");
    exit(0);
}
void handle_sigint(int signal) {
    // printf("current: %d\n",current_foreground_process_pid);
    // // // printf("Caught SIGINT\n");
    if (current_foreground_process_pid != -1) {
        // printf("Sending SIGINT to foreground process\n");
        kill(current_foreground_process_pid, SIGINT);
    }
    fflush(stdout);
    current_status = 0;   
}
void handle_sigstp(int signal) {
    // printf("Caught SIGTSTP\n");
    //     printf("current: %d\n",current_foreground_process_pid);
    if (current_foreground_process_pid != -1) {
        // printf("Sending SIGTSTP to foreground process\n");
        kill(current_foreground_process_pid, SIGTSTP);
        
    }
    fflush(stdout);
    return;
}

void ping(int pid, int signal) {
    int sig = signal % 32;
    int processs_cnt=0;
    int i=0;
    int flag=0;
    for(i=0;i<process_count;i++){
        if(processes[i].pid==pid && processes[i].status==0){
            processes[i].status=1;
            flag=1;
            break;
        }
    }
    // if(flag==0){
    //     printf("No such process\n");
    //     return;
    // }
    if (kill(pid, sig) == -1) {
        if (errno == ESRCH) {
            printf("No such process found\n");
        } else {
            perror("Error sending signal");
        }
    } else {
        printf("Sent signal %d to process with pid %d\n", sig, pid);
    }
    processs_cnt=0;
    int j=0;
    process_details* proces = get_processes(home_dir,shell_pid,&processs_cnt);
    for(int j=0;j<processs_cnt;j++){
       if(proces[j].pid==pid){
           processes[i].status = 0;
            break;
       } 
    }
    // printf("Signal sent\n");
    return;
}

#endif