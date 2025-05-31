#ifndef __BG_C__
#define __BG_C__

#include "bg.h"
#include "process.h"
#include<stdio.h>
#include<signal.h>
#include "activities.h"
#include "display.h"
#include<errno.h>
#include<stdlib.h>
void execute_bg_command(int pid){
    int i=0;
    int flag=0;
    for(i=0;i<process_count;i++){
        if(processes[i].pid==pid && processes[i].status==0){
            processes[i].status=1;
            flag =1;
            break;
        }
    }
    // if(flag==0){
    //     printf("No such process\n");
    //     current_status=1;
    //     return;
    // }
    
    if(kill(pid,SIGCONT)<0) 
    {
        if (errno == ESRCH) {
            printf("No such process found\n");
        } else {
            perror("Error sending signal");
        }
        return;
    }
    int processs_cnt=0;
    processes[i].status = 1;
    process_details* proces = get_processes(home_dir,shell_pid,&processs_cnt);
    for(int i=0;i<processs_cnt;i++){
       if(proces[i].pid==pid){
           processes[i].status = 0;
              break;
       }
       
    }
    free(proces);
    return;
}


#endif