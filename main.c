#include "display.h"
#include "log.h"
#include<stdio.h>
#include<string.h>
#include "background.h"
#include "foreground.h"
#include<stdlib.h>
#include<unistd.h>
#include "log.h"
#include <sys/wait.h>
#include "IO.h"
#include "pipe.h"
#include "sig.h"
#include "process.h"
// #include<signal.h>
// #include "activities.h"
int shell_pid;
char* home_dir;
int main() {
    char home[4096];
    getcwd(home, sizeof(home));
    initialize_process_list(4096);
    home_dir = home;
    printf("HOME directory is %s\n",home);
    int foreground = 0;
    char foreground_Command[4096];
    int pgid = getpgid(getpid());
    // printf("pgid: %d\n",pgid);
    int pid = getpid();
    printf("PID of the SHELL: %d\n",pid);
    shell_pid = pid;

    while(1){
        signal(SIGCHLD,handle_background_termination);
        signal(SIGINT,handle_sigint);
        signal(SIGTSTP,handle_sigstp);
        PrintUser(home,foreground,foreground_Command);
        foreground=0;
        char command[4096];
        if(fgets(command, sizeof(command), stdin) == NULL) {
            if (feof(stdin)) {
                // End-of-file (Ctrl-D)
                 handle_sigquit();
            } else if (ferror(stdin)) {
                // Error reading input
                perror("Error reading input");
                clearerr(stdin);  // Clear the error
                continue;
            }
            else{
                continue;
            }
        }
        log_command(command,home);
        char* token = strtok(command, ";");
        while(token != NULL) {
            token[strcspn(token, "\n")] = '\0';
                int n= strlen(token);
                int background = 0;
                int io_redirect=0;
                int pipe=0;
                for(int i=0;i<n;i++){
                    if(token[i]=='&'){
                        background = 1;
                        break;
                    }
                    else if(token[i]=='>' || token[i]=='<'){
                        io_redirect=1;
                    }
                    else if(token[i]=='|'){
                        pipe = pipe +1;
                    }

                }
                if(background){
                    make_agrs(token);
                    // printf("returning from background\n");
                }
                else if(io_redirect && !pipe){
                        redirect(token,home,pid);
                }
                else if(pipe){
                    pipe_handling(pipe,token,home,pid);
                }
                else{
                    foreground = foreground_execute(token,home,pid);
                    int i=0;
                    // printf("received;  %d\n",foreground);
                    if(foreground>0){
                        while(token[i]==' '){
                            i++;
                        }
                        while(token[i]!='\0' && token[i]!=' '){
                            foreground_Command[i]=token[i];
                            i++;
                        }
                        foreground_Command[i]='\0';
                        // printf("%s\n",foreground_Command);
                    }
                }
            token = strtok(NULL, ";");
    
        }
    }
    free(processes);

    return 0;
}
