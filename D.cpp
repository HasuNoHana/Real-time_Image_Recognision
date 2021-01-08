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

#include "dane.hpp"

void readMessage(char *fieldLength, char *fieldHeight, char mtext[]){
    int current=0;
    (*fieldLength) = mtext[0];
    (*fieldHeight) = mtext[1];
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

        char fieldLength, fieldHeight;
        readMessage(&fieldLength, &fieldHeight, buf.mtext);

        std::cout << "fieldLength: " << fieldLength;
        std::cout << " fieldHeight: " << fieldHeight << std::endl;

        i++;
        if (i >= 6) break;
    }
}