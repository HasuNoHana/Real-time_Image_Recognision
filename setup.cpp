#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>

//to co poniżej można by było przenieść do jakiegoś inego pliku .hpp (define oraz strukturę)
#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100


struct bufmsg{
    long mtype; /* 1 - z A do B; 2 - z B do A; 3 - z B do C; 4 - z C do D */
    char mtext[ROZMIAR_KOMUNIKATU];
};

int main() {

    //tworzenie kolejki i pamieci

    //utworzenie kolejki
    int id_kolejki = msgget(KLUCZ_KOLEJKA, IPC_CREAT|0666);
    if(id_kolejki==-1){
	std::cout<<"Blad otwarcia kolejki\n";
	return 1;
    }

    //utworzenie pamięci współdzielonej
    int id_pamieci = shmget(KLUCZ_PAMIEC, ROZMIAR_PAMIECI, IPC_CREAT|0666);
    if(id_pamieci==-1){
	std::cout<<"Blad utworzenia pamieci\n";
	msgctl(id_kolejki, IPC_RMID, NULL);
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
                        if(tpid == pidA) {
                            A_terminated = true;
                            std::cerr << "A terminated" << std::endl;
                        }
                        if(tpid == pidB) {
                            B_terminated = true;
                            std::cerr << "B terminated" << std::endl;
                        }
                        if(tpid == pidC) {
                            C_terminated = true;
                            std::cerr << "C terminated" << std::endl;
                        }
                        if(tpid == pidD) D_terminated = true;
                    } while(!A_terminated || !B_terminated || !C_terminated || !D_terminated);
                }
            }
        }
    }

    //usuwanie kolejki i pamięci

    //usunięcie kolejki
    msgctl(id_kolejki, IPC_RMID, NULL);
    //usunięcie pamięci
    shmctl(id_pamieci, IPC_RMID, NULL);

    return 0;
}
