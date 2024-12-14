#ifndef __FOREGROUND_C__
#define __FOREGROUND_C__
#include "foreground.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#include "custom.h"
#include "process.h"
// #include "log.h"
// #include "iMan.h"
// #include "activities.h"
void foreground(char* args[]){
    int fgp = fork();
    if(fgp<0)
    {
        perror("fork error");
        current_status =0;
        return;
    }
    if(fgp==0)
    {  
        // printf("%d\n",getpid());
        if(execvp(args[0],args)==-1){
            perror("execvp error");
            exit(1);
        }
        current_status = 0;
        exit(0);
    }
    else
    {
        int status1;
        current_foreground_process_pid = fgp;
        waitpid(fgp,&status1,WUNTRACED);
        if(WIFSTOPPED(status1)){
          processes[process_count].pid = fgp;
          processes[process_count].status = 0;
          process_count++;
          current_status = 0;
          current_foreground_process_pid = -1;
        }
        if(status1!=0){
            current_status = 0;
        }
        current_foreground_process_pid = -1;
        return;
    }
}

int foreground_execute(char *command,char* home,int pid){
        time_t start = time(NULL);
        run_command(command, home, pid);
        time_t end = time(NULL);
        int duration = (int)difftime(end, start);
        // printf("duration: %d\n",duration);
        if (duration > 2) {
                return duration;
        }
        else{
            return 0;
        }
    return 0;
}

#endif