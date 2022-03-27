#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
To compile: "gcc signals.c -o signals"
It will take 1 sec to show all the signals due to the use of sleep(1)
to catch the alarm() signal.
Here we created 6 different signals.
*/


void ABRT_handler(int signum){
    printf("Signal: Abort signal from abort()\n");
    fflush(stdout);
    exit(1);
}

void SEGV_handler(int signum){
    printf("Signal: Invalid memory reference\n");
    signal(SIGABRT, ABRT_handler);
    abort();
    fflush(stdout);
}

void FPE_handler(int signum){
    printf("Signal: Floating point exception - dividing by 0\n");
    signal(SIGSEGV, SEGV_handler);
    int *a;
    *a = 0;
    fflush(stdout);
}

void USR1_handler(int signum){
    printf("Signal: User defined 1\n");
    signal(SIGFPE,FPE_handler);
    int a = 0;
    int b = 13;
    int c = b/a;
    fflush(stdout);
}

void CHLD_handler(int signum){
    printf("Signal: Child terminated\n");
    signal (SIGUSR1, USR1_handler);
    raise(SIGUSR1);
    fflush(stdout);
}

void ALRM_handler(int signum){
    printf("Signal: Timer signal from alarm(2)\n");
    int status;
    signal (SIGCHLD, CHLD_handler);
    if (!(fork())) {
        exit(1);
    }
    wait(&status);
    fflush(stdout);
}

int main(){
    signal(SIGALRM, ALRM_handler);
    alarm(1);
    sleep(1);
    return 0;
}