#ifndef __CUSTOM_C__
#define __CUSTOM_C__
#include "foreground.h"
#include "iMan.h"
#include "log.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include<sys/wait.h>
#include<time.h>
#include "seek.h"
#include "myshrc.h"
#include "activities.h"
#include "sig.h"
#include "custom.h"
#include "foreground.h"
#include "background.h" 
#include "pipe.h"
#include "IO.h"
#include "fg.h"
#include "bg.h"
#include"iMan.h"
#include "neonate.h"
void log_utility(const char* command_local,char* home,int pid){ 
        char* command= strdup(command_local);
        char* saveptr;
        char* token = strtok_r(command, ";",&saveptr);
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
                    int random = foreground_execute(token,home,pid);
        
                }
            token = strtok_r(NULL, ";",&saveptr);
        }
        return ;
}
void run_command(const char* command_local, char* home,int pid){
    char *command = strdup(command_local);
    // printf("iam here\n");
    // printf("%s\n",command);
    char *saveptr;
    char *token = strtok_r(command, " ", &saveptr);
        if(token == NULL) {
            // printf("Invalid Command\n");
            return;
        }
        if(strcmp(token,"hop")==0){
            //   printf("hop\n"); 
                token=strtok_r(NULL," ",&saveptr);
                if(token==NULL){
                    hop(home,home);
                    return;
                }

                while(token!=NULL){
                    token[strcspn(token, "\n")] = '\0';
                    hop(token,home);
                    token=strtok_r(NULL," ",&saveptr);
                }
        }
        else if(strcmp(token,"reveal")==0){
            int flag_l=0;
            int flag_a=0;
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                reveal(".",home,0,0);
                return;
            }
            while(token!=NULL){
                    token[strcspn(token, "\n")] = '\0';
                    if(token[0]=='-'){
                        for(int j=1;j<strlen(token);j++){
                            if(token[j]=='l'){
                                flag_l=1;
                            }
                            else if(token[j]=='a'){
                                flag_a=1;
                            }
                            else{
                                continue;
                            }
                        }
                    }
                    else{
                        break;
                    }
                    token=strtok_r(NULL," ",&saveptr);
            }
            if(token!=NULL){
                token[strcspn(token, "\n")] = '\0';
                reveal(token,home,flag_l,flag_a);
            }
            else{
                reveal(".",home,flag_l,flag_a);
            }
        }
        else if(strcmp(token,"log")==0){
               token=strtok_r(NULL," ",&saveptr);
                char path[4096];  
                sprintf(path, "%s/log.txt", home);

                if(token==NULL){
                     log_print(path);
                     return;
                }
                while(token!=NULL){
                    token[strcspn(token, "\n")] = '\0';
                    if(strcmp(token,"purge")==0){
                        log_purge(path);
                        return;
                    }
                    else if(strcmp(token,"execute")==0){
                        token=strtok_r(NULL," ",&saveptr);
                        char ch[4096];
                        // printf("%d\n",atoi(token));
                        if(get_command(path,atoi(token))!=NULL){
                            strcpy(ch, get_command(path,atoi(token)));
                            // printf("%s\n",ch);
                            // run_command(ch, home,pid);
                            log_utility(ch,home,pid);
                            return;
                        }
                        else{
                             return;
                        }
                    }
                    else{
                        printf("Invalid Command\n");
                    }
                    token=strtok_r(NULL," ",&saveptr);
                }
        }
        else if(strcmp(token,"proclore")==0){
            // printf("proclore\n");
               token=strtok_r(NULL," ",&saveptr);
                if(token==NULL){
                     int pid = getpid();
                        proclore(pid,home);
                     return;

                 }
                while(token!=NULL){
                    token[strcspn(token, "\n")] = '\0';
                    proclore(atoi(token),home);
                    token=strtok_r(NULL," ",&saveptr);
                }
                return;

        }
        else if(strcmp(token,"seek")==0){
            char *args[4096];
            int i=0;
            while(token!=NULL){
                        token[strcspn(token, "\n")] = '\0';
                        // printf("%s\n",token);
                        args[i++]=token;
                        token=strtok_r(NULL," ",&saveptr);
            }
            seek(i,args,home);
            return;
        }
        else if(strcmp(token,"activities")==0){
            // while(1){
            //     activity(home,getpid());
            // }
            activity(home,getpid());
        }
        else if(strcmp(token,"ping")==0){
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            int proc_id = atoi(token);
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            int signal = atoi(token);
            ping(proc_id,signal);
        }
        else if(strcmp(token,"fg")==0){
            // printf("fg\n");
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            int pid = atoi(token);
            // token=strtok_r(NULL," ",&saveptr);
            // if(token==NULL){
                execute_fg_command(pid);
                return;
            // }
        }
        else if(strcmp(token,"bg")==0){
            // printf("fg\n");
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            int pid = atoi(token);
            // token=strtok_r(NULL," ",&saveptr);
            // if(token==NULL){
                execute_bg_command(pid);
                return;
            // }
        }
        else if(strcmp(token,"iMan")==0){
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            fetch_manpage(token);
            // printf("%s\n",token);
        }
        else if(strcmp(token,"neonate")==0){
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            if(strcmp(token,"-n")==0){
                token = strtok_r(NULL," ",&saveptr);
                if(token==NULL){
                    printf("Invalid Command\n");
                    return;
                }
                int time = atoi(token);
                neonate(time);
            }
            else{
                printf("Invalid Command\n");
            }
        }
        else if(strcmp(token,"mk_hop")==0){
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            char *args[4096];
            args[0]="mkdir";
            args[1]=token;
            args[2]=NULL;
            foreground(args);
            hop(token,home);
            return;
        }
        else if(strcmp(token,"hop_seek")==0){
            token=strtok_r(NULL," ",&saveptr);
            if(token==NULL){
                printf("Invalid Command\n");
                return;
            }
            hop(token,home);
            char *args[4096];
            args[0]=token;
            args[1]=token;
            seek(2,args,home);
            return;
        }
        else if(strcmp(token,"exit")==0){
            exit(0);
        }
        else{
            if(search(home,command,pid)==0){
                char *args[4096];
                int i=0;
                while(token!=NULL){
                            token[strcspn(token, "\n")] = '\0';
                            // printf("%s\n",token);
                            args[i++]=token;
                            token=strtok_r(NULL," ",&saveptr);
                }
                args[i]=NULL; 
                foreground(args);
                return;
            }
              
        }         
        return;
}

#endif