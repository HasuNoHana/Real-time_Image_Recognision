#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/msg.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h>

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
    int length = rand() % 3 + 1, height = rand() % 3 + 1; //get random numbers from 1 to 3
    switch (length) {
        case 1:
            field.fieldLength = 'A';
            break;
        case 2:
            field.fieldLength = 'B';
            break;
        case 3:
            field.fieldLength = 'C';
            break;
        default:
            std::cerr << "Wrong random number";
            exit(1);
    }
    field.fieldHeight = char(height+48);
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

void printWhatField(Field currentField){
    std::cout << "Point at field: " << currentField.fieldLength << currentField.fieldHeight<<std::endl;
}

void welcomeMessage(){
    std::cout << "*** Welcome to our fabulous game! ***" << std::endl;
    std::cout << "Rules Are Simple, light up your flashlight and point at appropriate field." << std::endl;
    std::cout << "  _______" << std::endl;
    std::cout << "3 |_|_|_|" << std::endl;
    std::cout << "2 |_|_|_|" << std::endl;
    std::cout << "1 |_|_|_|" << std::endl;
    std::cout << "   A B C " << std::endl;
    std::cout << "Ready, Steady Go!" << std::endl;
}

int main(int argc, char** argv) {
    srand (time(NULL));
    //otwarcie kolejki
    int queue_id = msgget(KLUCZ_KOLEJKA, 0);
    if(queue_id==-1){
        error_message_exit("Error while opening message queue", nullptr);
    }

    bufmsg buf;
    bool lastFieldAcomplished = false, recivedIsNotCurrent = true;
    Field currentField, recivedField;

    int i = 0, totalPoints = 0;

    //uncoment after tests
//    welcomeMessage();

    while (true) {
        currentField = generateField();
        printWhatField(currentField);
        auto start = std::chrono::high_resolution_clock::now();//start counting time

        while(recivedIsNotCurrent){//runs until good field is pointed to
            if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 4, 0) == -1) {
                std::cerr << "Error while receiving message from C" << std::endl;
                return 1;
            }
            // std::cerr << "message from C received" << std::endl;
            std::cerr << buf.mtext << std::endl;

            readMessage(&recivedField, buf.mtext);

            if(checkIfFieldCorrect(recivedField,currentField)) {
                recivedIsNotCurrent = false;
                auto finish = std::chrono::high_resolution_clock::now();//finish counting time
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

                int currentPoints = countPoints(duration.count());
                totalPoints+=currentPoints;
                printPositiveFeedback(currentPoints,totalPoints);
                srand (time(NULL));//seed
            }
            else{
                recivedIsNotCurrent = true;
            }
        }
        i++;
        if (i >= 6) break;
    }
    // notify A that the program has ended
    buf.mtype = 5;
    strncpy(buf.mtext, "", 1);
    if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
        std::cerr << "Error while sending the final message to A" << std::endl;
    }
    // notify B that the program has ended
    buf.mtype = 6;
    strncpy(buf.mtext, "", 1);
    if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
        std::cerr << "Error while sending the final message to B" << std::endl;
    }
    // notify C that the program has ended
    buf.mtype = 7;
    strncpy(buf.mtext, "", 1);
    if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
        std::cerr << "Error while sending the final message to C" << std::endl;
    }
}