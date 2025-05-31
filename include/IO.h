#ifndef __IO_H__
#define __IO_H__
void redirect(char* command,char* home,int pid);
char* args_to_string(char* args[], int num_args);
#endif
