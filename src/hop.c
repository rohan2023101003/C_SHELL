#ifndef __HOP_C__
#define __HOP_C__
#include "hop.h"
#include "display.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
char prev[4096]=".";
void PrintPath(){
    char cur[4096];
    getcwd(cur, sizeof(cur));
    printf("%s\n",cur);
    return;
}
char* prev_dir(){
    return prev;
}
void hop(char* newdir,char* home){
    // printf("hop\n");
    if(strcmp(newdir,"-")==0){
        char temp[4096];
        getcwd(temp, sizeof(temp));
        if(chdir(prev)==-1){
            printf("No such directory\n");
            return;
        }
        strcpy(prev,temp);
        PrintPath();
        return;
    }
    getcwd(prev, sizeof(prev));
    if(newdir[0]=='~' && newdir[1]=='/' && strlen(newdir)>2){
        char abs_path[4096];
        strcpy(abs_path,home);
        replaceSubstring(newdir, "~", "");
        strcat(abs_path,newdir);
        if(chdir(abs_path)==-1){
            printf("No such directory\n");
            return;
        }
    }
    else if(strcmp(newdir,"~")==0){
        if(chdir(home)==-1){
            printf("No such directory\n");
            return;
        }
    }
    else if(strcmp(newdir,"hop\n")==0 || strcmp(newdir,"\n")==0){
        if(chdir(home)==-1){
            printf("No such directory\n");
            return;
        }
    }
    else{
        if(chdir(newdir)==-1){
            printf("No such directory\n");
            return;
        }
    }
    // printf("reached here\n");
    PrintPath();
    return;
}
#endif