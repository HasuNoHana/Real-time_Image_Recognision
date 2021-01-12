#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>

#include "dane.hpp"

int main() {

    //tworzenie kolejki i pamieci

    //utworzenie kolejki 1
    int id_kolejki_1 = msgget(KLUCZ_KOLEJKA_1, IPC_CREAT|0600);
    if(id_kolejki_1==-1){
	std::cout<<"Blad otwarcia kolejki\n";
	return 1;
    }
    //utworzenie kolejki 2
    int id_kolejki_2 = msgget(KLUCZ_KOLEJKA_2, IPC_CREAT|0600);
    if(id_kolejki_2==-1){
	std::cout<<"Blad otwarcia kolejki\n";
	return 1;
    }

    //utworzenie pamięci współdzielonej
    int id_pamieci = shmget(KLUCZ_PAMIEC, ROZMIAR_PAMIECI, IPC_CREAT|0600);
    if(id_pamieci==-1){
	std::cout<<"Blad utworzenia pamieci\n";
	msgctl(id_kolejki_1, IPC_RMID, NULL);
	msgctl(id_kolejki_2, IPC_RMID, NULL);
	return 1;
    }

    //tworzenie procesów

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

    //usuwanie kolejki i pamięci

    //usunięcie kolejki
    msgctl(id_kolejki_1, IPC_RMID, NULL);
    msgctl(id_kolejki_2, IPC_RMID, NULL);
    //usunięcie pamięci
    shmctl(id_pamieci, IPC_RMID, NULL);

    return 0;
}
