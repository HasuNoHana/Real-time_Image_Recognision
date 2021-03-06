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

inline void error_message_exit(const char *message, uchar *shared_frame) {
    std::cerr << message << ": " << strerror(errno)  << std::endl;
    exit(1);
}

int main(int argc, char** argv) {
    //otwarcie kolejki
    int queue_id = msgget(KLUCZ_KOLEJKA, 0);
    if(queue_id==-1){
        error_message_exit("Error while opening message queue", nullptr);
    }

    bufmsg buf;

    int i = 0;
    while (true) {

        if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
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