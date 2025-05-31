#ifndef __SEEK_C__
#define __SEEK_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include"display.h"
#define MAX_PATHS 1000
void read_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return;
    }
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    return;
}
void search_directory(const char *target1, const char *search,char** path_array,int* path_count,char* home) {
    char target[4096];
    strcpy(target,target1);
    if(target[0]=='~' && target[1]=='/' && strlen(target)>=2){
        char abs_path[4096];
        strcpy(abs_path,home);
        replaceSubstring(target, "~", "");
        strcat(abs_path,target);
        strcpy(target,abs_path);
    }
    else if(strcmp(target,"~")==0){
        strcpy(target, home);
    }
    else if(strcmp(target,".")==0){
        char buf[4096];
        getcwd(buf, sizeof(buf));
        strcpy(target,buf);
    }
    DIR *dir=opendir(target);
    if (!dir) {
        perror("opendir");
        return;
    }
    struct dirent **entry;
    struct stat fileStat;
    // printf("%s\n", target);
    int n;
    n = scandir(target,&entry , NULL, alphasort);
    if (n == -1) {
        perror("scandir error");
        return;
    }
    for (int i = 0; i < n; i++) {
        char path[10000];
        snprintf(path, sizeof(path), "%s/%s", target, entry[i]->d_name);
        if (strcmp(entry[i]->d_name, ".") == 0 || strcmp(entry[i]->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(entry[i]->d_name, search) == 0) {
                if (*path_count < MAX_PATHS) {
                    path_array[*path_count] = strdup(path);  // Copy the path to the array
                    (*path_count)++;
                } else {
                    fprintf(stderr, "Path array is full!\n");
                    return;
                }
        }
        if (entry[i]->d_type == DT_DIR) {
            search_directory(path, search, path_array, path_count, home);
        }
        free(entry[i]);
    }
    free(entry);
    closedir(dir);
    return; 
}
void seek(int argc,char *argv[],char* home) {
    int only_dirs = 0;
    int only_files = 0;
    int enable_action = 0;
    char *search_term = NULL;
    char *target_directory = ".";
    // printf("%d\n",argc);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            only_dirs = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            only_files = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            enable_action = 1;
        } else if (!search_term) {
            search_term = argv[i];
        } else {
            target_directory = argv[i];
        }
    }
    if(search_term==NULL){
        printf("Invalid Command\n");
        return;
    }
    // printf("%s %s\n",search_term,target_directory);
    if (only_dirs && only_files) {
        printf("Invalid flags!\n");
        return ;
    }
    int path_count = 0;
    // char found_path[4096];
    char *path_array[MAX_PATHS]; 
    for (int i = 0; i < MAX_PATHS; i++) {
        path_array[i] = NULL;
    }
    search_directory(target_directory, search_term, path_array,&path_count,home);
    if(path_count==0){
        printf("No match found!\n");
        return;
    }
    // printf("Found %d matches:\n", path_count);
    for (int i = 0; i < path_count; i++) {
    //    printf("%s\n", path_array[i]);
       struct stat path_stat;
            if (stat(path_array[i], &path_stat) != 0) {
                perror("stat");
                return;
            }
            if(enable_action){
                if(path_count!=1){
                    printf("More than one match found!\n");
                    return;
                }
                if(S_ISDIR(path_stat.st_mode) && only_dirs){
                        printf("\033[34m%s\033[0m\n", path_array[i]);
                        if (enable_action && access(path_array[i], X_OK) != 0) {
                            printf("Missing permissions for task!\n");
                        } else {
                            if (chdir(path_array[i]) == -1) {
                                perror("chdir");
                            }
                        }
                }
                else if(S_ISREG(path_stat.st_mode) && only_files){
                        printf("\033[32m%s\033[0m\n", path_array[i]);
                        if(enable_action){
                            read_file(path_array[i]);
                        }
                }
                else{
                        if(S_ISDIR(path_stat.st_mode)){
                            printf("\033[34m%s\033[0m\n", path_array[i]);
                            if (enable_action && access(path_array[i], X_OK) != 0) {
                                printf("Missing permissions for task!\n");
                            } else {
                                if (chdir(path_array[i]) == -1) {
                                    perror("chdir");
                                }
                            }
                        }
                        else if(S_ISREG(path_stat.st_mode)){
                            printf("\033[32m%s\033[0m\n", path_array[i]);
                            if(enable_action){
                                read_file(path_array[i]);
                            }
                        }
                    }
                    break;
                }
                if(S_ISDIR(path_stat.st_mode)  && only_dirs){
                            printf("\033[34m%s\033[0m\n", path_array[i]);
                }
                else if(S_ISREG(path_stat.st_mode) && only_files){
                    printf("\033[32m%s\033[0m\n", path_array[i]);

                }
                else if(!only_dirs && !only_files){
                    if(S_ISDIR(path_stat.st_mode)){
                        printf("\033[34m%s\033[0m\n", path_array[i]);
                    }
                    else if(S_ISREG(path_stat.st_mode)){
                        printf("\033[32m%s\033[0m\n", path_array[i]);
                    }

                }
    }
    return ;
}
#endif