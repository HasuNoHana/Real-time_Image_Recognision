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

void readField(char *fieldLength, char *fieldHeight, char mtext[]){
    int current=0;

    (*fieldLength) = mtext[0];
    (*fieldHeight) = mtext[1];
//
//    while(( int(mtext[current]) != 0 ) && (current < ROZMIAR_KOMUNIKATU)){
//        std::cout << int(mtext[current]) << " ";
//    if( mtext[current] == ' ' ){
//    results[currentResault] = readLastNumber(mtext, current, lastSpace);
//    lastSpace = current;
//    currentResault++;
//    }
//    current++;
//    }
//    results[currentResault] = readLastNumber(mtext, current, lastSpace);

}

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

//        for(int j=0; j < ROZMIAR_KOMUNIKATU; j++){
//            std::cout << int(buf.mtext[j]) << " ";
//        }

        char fieldLength, fieldHeight;
        readField(&fieldLength, &fieldHeight, buf.mtext);

        std::cout << " fieldLength: " << fieldLength;
        std::cout << " fieldHeight: " << fieldHeight << std::endl;

        i++;
        if (i >= 6) break;
    }
}