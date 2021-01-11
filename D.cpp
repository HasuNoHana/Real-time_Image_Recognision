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
    if(recivedField.fieldLength != currentField.fieldLength){
        return false;
    }
    if(recivedField.fieldHeight != currentField.fieldHeight){
        return false;
    }
    return true;
}

int countPoints(int duration){
    int averrageDelay = 400, points = 0;
    if( 0 <= duration && duration < 500 + averrageDelay){
        points = 10000;
    } else if( 500 + averrageDelay <= duration && duration < 1000 + averrageDelay){
        points = 8000;
    } else if( 1000 + averrageDelay <= duration && duration < 3000 + averrageDelay){
        points = 5000;
    } else if( 3000 + averrageDelay <= duration && duration < 5000 + averrageDelay){
        points = 3000;
    } else if( 5000 + averrageDelay <= duration && duration < 8000 + averrageDelay){
        points = 1000;
    } else if( 8000 + averrageDelay <= duration && duration < 10000 + averrageDelay){
        points = 500;
    } else {
        points = 0;
    }
    return points;
}

void printPositiveFeedback(int currentPoints, int totalPoints){
    std::cout << "Your current score is ";
    switch (currentPoints) {
        case 10000:
            std::cout << "10000! You are Superman!";
            break;
        case 8000:
            std::cout << "8000! You are SuperHiper!";
            break;
        case 5000:
            std::cout << "5000! You are Great!";
            break;
        case 3000:
            std::cout << "3000! You are Good";
            break;
        case 1000:
            std::cout << "1000. You are Fine";
            break;
        case 500:
            std::cout << "500. You are Weak";
            break;
        case 0:
            std::cout << "0. You are Dead";
            break;
        default:
            std::cout << "Wrong score";
    }
    std::cout << std::endl;
    std::cout << "Your total score is " << totalPoints << "!" << std::endl;
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

    int i = 0, totalPoints = 0;

    while (true) {

        currentField = generateField();
        auto start = std::chrono::high_resolution_clock::now();

        while(recivedIsNotCurrent){
            if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
                std::cerr << "Error while receiving message from C" << std::endl;
                return 1;
            }
            std::cerr << "message from C received" << std::endl;

            readMessage(&recivedField, buf.mtext);

//            std::cout << "fieldLength: " << recivedField.fieldLength;
//            std::cout << " fieldHeight: " << recivedField.fieldHeight << std::endl;

            if(checkIfFieldCorrect(recivedField,currentField)) {
                recivedIsNotCurrent = false;
                auto finish = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
                int currentPoints = countPoints(duration.count());
                totalPoints+=currentPoints;
                printPositiveFeedback(currentPoints,totalPoints);
            }
            else{
                recivedIsNotCurrent = true;
            }

        }

        i++;
        if (i >= 6) break;
    }
}