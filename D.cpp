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

struct Field{
    char fieldLength;//A,B,C
    char fieldHeight;//1,2,3
};

void readMessage(Field *recivedField, char mtext[]){
    int current=0;
    (*recivedField).fieldLength = mtext[0];
    (*recivedField).fieldHeight = mtext[1];
}

inline void error_message_exit(const char *message, uchar *shared_frame) {
    std::cerr << message << ": " << strerror(errno)  << std::endl;
    exit(1);
}

Field generateField() {
    Field field;
    field.fieldLength = 'B';
    field.fieldHeight = '1';
    return field;
}

bool checkIfFieldCorrect(Field recivedField, Field currentField) {
    if(recivedField.fieldLength != currentField.fieldHeight)
        return false;
    if(recivedField.fieldHeight != currentField.fieldHeight)
        return false;
    return true;
}

int main(int argc, char** argv) {
    //otwarcie kolejki
    int queue_id = msgget(KLUCZ_KOLEJKA, 0);
    if(queue_id==-1){
        error_message_exit("Error while opening message queue", nullptr);
    }

    bufmsg buf;
    bool lastFieldAcomplished = false, recivedIsNotCurrent = true;
    Field currentField, recivedField;

    int i = 0;

    while (true) {

        currentField = generateField();
        auto start = std::chrono::high_resolution_clock::now();

        while(recivedIsNotCurrent){
            if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
                std::cerr << "Error while receiving message from C" << std::endl;
                return 1;
            }
            std::cerr << "message from C received" << std::endl;

            char fieldLength, fieldHeight;
            readMessage(&recivedField, buf.mtext);

            std::cout << "fieldLength: " << recivedField.fieldLength;
            std::cout << " fieldHeight: " << recivedField.fieldHeight << std::endl;

            recivedIsNotCurrent = !checkIfFieldCorrect(recivedField,currentField);

            std::cout << " recivedIsNotCurrent: " << int(recivedIsNotCurrent);
            std::cerr << " recivedIsNotCurrent: " << recivedIsNotCurrent;

        }
//        std::cerr << " recivedIsNotCurrent: " << recivedIsNotCurrent;
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

        std::cout << " duration: " << duration.count() << std::endl;
        std::cerr << " duration: " << duration.count() << std::endl;

        i++;
        if (i >= 6) break;
    }


//    while (true) {
//
//        if(!lastFieldAcomplished){
//            currentField = generateField();
//            auto start = std::chrono::high_resolution_clock::now();
//        }
//
//        if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
//            std::cerr << "Error while receiving message from C" << std::endl;
//            return 1;
//        }
//        std::cerr << "message from C received" << std::endl;
//
//        char fieldLength, fieldHeight;
//        readMessage(&recivedField, buf.mtext);
//
//        lastFieldAcomplished = checkIfFieldCorrect(recivedField,currentField);
//
//        if(lastFieldAcomplished){
//            auto finish = std::chrono::high_resolution_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
//        }
//
//        std::cout << "fieldLength: " << recivedField.fieldLength;
//        std::cout << " fieldHeight: " << recivedField.fieldHeight << std::endl;
//
//        i++;
//        if (i >= 6) break;
//    }
}