#ifndef __SEEK_C__
#define __SEEK_C__
void search_directory(const char *target1, const char *search,char** path_array,int* path_count,char* home) ;
void seek(int argc,char *argv[],char* home);
void read_file(const char *pat);
#endif