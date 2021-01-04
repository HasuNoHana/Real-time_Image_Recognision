#include<iostream>
#include <opencv2/opencv.hpp>
#include <fcntl.h>  
#include <sys/stat.h>
#include <mqueue.h>
#include <string>

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

//to co poniżej można by było przenieść do jakiegoś inego pliku .hpp (define oraz strukturę)
#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100


struct bufmsg{
    long mtype; /* 1 - z A do B; 2 - z B do A; 3 - z B do C; 4 - z C do D */
    char mtext[ROZMIAR_KOMUNIKATU];
};

int main(int argc, char** argvdd) {

    //otwarcie kolejki
    int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
    if(id_kolejki==-1){
        std::cout<<"Blad otwarcia kolejki\n";
        return 1;
    }

    std::cout<<"C Jest\n";

    bufmsg buf;
    int i = 0;
    while (true) {
        buf.mtype = 3;
        strncpy(buf.mtext, "w", ROZMIAR_KOMUNIKATU);
        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to A" << std::endl;
            return 1;
        }
        std::cerr << "message to A sent" << std::endl;
        if (msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 1, 0) == -1) {
            std::cerr << "Error while receiving message from A" << std::endl;
            return 1;
        }
        std::cerr << "message from A received" << std::endl;
        // shared memory handling
        read_from_file(src, argc, argv); //mock, to be deleted

        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY); // Convert the image to Gray
        locate(dst, x, y, src);

        buf.mtype = 3;
        strncpy(buf.mtext, format_message(x, y).c_str(), ROZMIAR_KOMUNIKATU);

        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to C" << std::endl;
            return 1;
        }
        i++;
        if (i >= 6) break;
    }
}