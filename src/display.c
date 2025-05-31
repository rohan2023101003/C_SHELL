#ifndef __DISPLAY_C__
#define __DISPLAY_C__
#include "display.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pwd.h>
#include<stdlib.h>
void replaceSubstring(char *str, const char *old_sub, const char *new_sub) {
    char buffer[1024];
    char *pos;
    size_t old_len = strlen(old_sub);
    size_t new_len = strlen(new_sub);
    buffer[0] = '\0';
    while ((pos = strstr(str, old_sub)) != NULL) {
        strncpy(buffer, str, pos - str);
        buffer[pos - str] = '\0';
        strcat(buffer, new_sub);
        strcat(buffer, pos + old_len);
        strcpy(str, buffer);
    }
}
void PrintUser(char* home,int foreg,char* command){
    int user_id = (int)getuid();
    struct passwd *pw = getpwuid(user_id);
    char systemname[4096];
    gethostname(systemname, sizeof(systemname));
    char *username = pw->pw_name;
    char cur[4096];
    getcwd(cur, sizeof(cur));
    replaceSubstring(cur, home, "~");
    if(foreg<=0){
      printf("\033[1;35m<%s@%s:%s>\033[0m",username,systemname,cur);
    }
    else{
        // printf("%s\n",command);
       printf("\033[1;35m<%s@%s:%s %s : %d >\033[0m",username,systemname,cur,command,foreg);
    }
    return;
}
#endif