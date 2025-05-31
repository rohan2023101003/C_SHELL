#ifndef __NEONATE_C__
#define __NEONATE_C__
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <termios.h>

#define BUFFER_SIZE 1024

int get_most_recent_pid() {
    char path[] = "/proc/loadavg";
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /proc/loadavg");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        perror("Failed to read /proc/loadavg");
        close(fd);
        return -1;
    }
    buffer[bytesRead] = '\0'; 
    close(fd);

    char *saveptr;
    char *token = strtok_r(buffer, " ", &saveptr);
    int field = 0;
    int recent_pid = -1;

    while (token != NULL) {
        if (field == 4) { // The 5th field contains the most recent PID
            recent_pid = atoi(token);
            break;
        }
        token = strtok_r(NULL, " ", &saveptr);
        field++;
    }

    return recent_pid;
}


int termination(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
void neonate(int time_arg) { 
     if (time_arg <= 0) {
        printf("Error: time_arg must be greater than 0.\n");
        return;
    }
    
    while (1) {
        int recent_pid = get_most_recent_pid();
        if (recent_pid != -1) {
            printf("%d\n", recent_pid);
        } else {
            printf("No processes found.\n");
        }
        int a = time_arg * 10;
        printf("a: %d\n", a);
        for (int i = 0; i < a; i++) {
            printf("%d\n", i);
            if (termination() && getchar() == 'x') {
                return ;
            }
            usleep(100000); // Sleep for 100ms
        }
    }
    return;
}

#endif