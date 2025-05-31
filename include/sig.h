#ifndef __SIG_H__
#define __SIG_H__
void ping(int pid,int signal);
void handle_sigquit();
void handle_background_termination(int sigal);
void handle_sigint(int signal);
void handle_sigstp(int signal);
#endif