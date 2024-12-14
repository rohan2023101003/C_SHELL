#ifndef __REVEAL_C__
#define __REVEAL_C__
#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include "reveal.h"
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include "display.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
void printList( mode_t mode,char* name){
        if (S_ISDIR(mode)) {
            printf("\033[1;34m\t%s\n\033[0m", name);
        } else if (S_ISREG(mode)) {
            if (mode & S_IXUSR) {
                printf("\033[1;32m\t%s\n\033[0m",name);
            } else {
                printf("\033[1;37m\t%s\n\033[0m", name);
            }
        } else if (S_ISLNK(mode)) {
            printf("\t%s\n", name);
        } else {
            printf("\t%s\n", name);
        }
        return;
}
void printPermissions(mode_t mode) {
        printf((S_ISDIR(mode)) ? "d" : "-");
        printf((mode & S_IRUSR) ? "r" : "-");
        printf((mode & S_IWUSR) ? "w" : "-");
        printf((mode & S_IXUSR) ? "x" : "-");
        printf((mode & S_IRGRP) ? "r" : "-");
        printf((mode & S_IWGRP) ? "w" : "-");
        printf((mode & S_IXGRP) ? "x" : "-");
        printf((mode & S_IROTH) ? "r" : "-");
        printf((mode & S_IWOTH) ? "w" : "-");
        printf((mode & S_IXOTH) ? "x" : "-");
}
void reveal(char* path,char* home,int l,int a){
    struct dirent **namelist;
    // printf("%s %s\n",path,home);
    if(path[0]=='~' && path[1]=='/' && strlen(path)>=2){
        char abs_path[4096];
        strcpy(abs_path,home);
        replaceSubstring(path, "~", "");
        // printf("%s\n",path);
        strcat(abs_path,path);
        path=abs_path;
    }
    else if(path==NULL || strcmp(path,"\0")==0 || strcmp(path," ")==0 || strcmp(path,"\n")==0){
        path=".";
    }
    else if(strcmp(path,"~")==0 ){
        // printf("%s\n",home);
        path= home;
    }
    else{
        //do nothing
    }
    // printf("%s\n",path);
    DIR *dir;
    struct dirent **entry;
    struct stat fileStat;
    if ((dir = opendir(path)) == NULL) {
        perror("opendir error");
        return;
    }
    // printf("%s\n", path);
    int n;
    n = scandir(path,&entry , NULL, alphasort);
    if (n == -1) {
        perror("scandir error");
        return;
    }
    printf("%d\n",n);
    char fullPath[4352];
    int i=0;
    while (i<n) {
        if (entry[i]->d_name[0] == '.' && a==0) {
            i++;
            continue;
        }
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry[i]->d_name);
        if(l==1){
            if (stat(fullPath, &fileStat) == 0) {
                printPermissions(fileStat.st_mode);
                printf(" %ld", fileStat.st_nlink);
                printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
                printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
                printf(" %ld", fileStat.st_size);
                char timeBuf[20];
                struct tm* timeinfo = localtime(&fileStat.st_mtime);
                strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", timeinfo);
                printf(" %s", timeBuf);
            }
        }
        if (stat(fullPath, &fileStat) == 0) {
            printList(fileStat.st_mode,entry[i]->d_name);
        }
        i++;
    }
    closedir(dir);
}
#endif