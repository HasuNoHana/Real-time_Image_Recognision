#include <iostream>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100

struct bufmsg{
    long mtype; // 1: A->B, 2: B->A, 3: B->C, 4: C->D
    char mtext[ROZMIAR_KOMUNIKATU];
};

int main(int argc, char** argv) {
    //otwarcie kolejki
    int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
    if(id_kolejki==-1){
        std::cout<<"Blad otwarcia kolejki\n";
        return 1;
    }

    bufmsg buf;

    int i = 0;
    while (true) {

        if (msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
            std::cerr << "Error while receiving message from C" << std::endl;
            return 1;
        }
        std::cerr << "message from C received" << std::endl;

        for(int j=0; j < ROZMIAR_KOMUNIKATU; j++){
            std::cout << int(buf.mtext[j]) << " ";
        }

        i++;
        if (i >= 6) break;
    }
}