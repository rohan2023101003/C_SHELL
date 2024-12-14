#ifndef __LOG_H__
#define __LOG_H__
void log_command(char* command,char* home); 
int count_commands(char* path);
void log_print(char* path);
void log_purge(char* path);
int check_log(char* command);
char* get_command(char* path,int t);
void skip_first_line(char* home);
#endif