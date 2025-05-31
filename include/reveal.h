#ifndef __REVEAL_H__
#define __REVEAL_H__
#include <sys/types.h>
void reveal(char* path,char* home,int l,int a);
void printList( mode_t mode,char* name);
void printPermissions(mode_t mode);
#endif