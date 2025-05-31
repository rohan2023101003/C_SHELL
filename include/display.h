#ifndef __DISPLAY_H__
#define __DISPLAY_H__
void PrintUser(char* home,int foreg,char* command);
void replaceSubstring(char *str, const char *old_sub, const char *new_sub);
extern int shell_pid;
extern char* home_dir;
#endif