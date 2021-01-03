#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>

int main() {
    int child_status;
    pid_t pidA = fork();
    if (pidA == 0) {
        char arg[] = {'A'};
        static char *argv[]={arg, NULL};
        execv("./A", argv);
        exit(EXIT_FAILURE);
    } else {
        pid_t pidB = fork();
        if (pidB == 0) {
            char arg[] = {'B'};
            static char *argv[]={arg, NULL};
            execv("./B", argv);
            exit(EXIT_FAILURE);
        } else {
            pid_t pidC = fork();
            if (pidC == 0) {
                char arg[] = {'C'};
                static char *argv[]={arg, NULL};
                execv("./C", argv);
                exit(EXIT_FAILURE);
            } else {
                pid_t pidD = fork();
                if (pidD == 0) {
                    char arg[] = {'D'};
                    static char *argv[]={arg, NULL};
                    execv("./D", argv);
                    exit(EXIT_FAILURE);
                } else {
                    pid_t tpid;
                    bool A_terminated = false, B_terminated = false, C_terminated = false, D_terminated = false;
                    do {
                        pid_t tpid = wait(&child_status);
                        if(tpid == pidA) A_terminated = true;
                        if(tpid == pidB) B_terminated = true;
                        if(tpid == pidC) C_terminated = true;
                        if(tpid == pidD) D_terminated = true;
                    } while(!A_terminated || !B_terminated || !C_terminated || !D_terminated);
                }
            }
        }
    }
    return 0;
}