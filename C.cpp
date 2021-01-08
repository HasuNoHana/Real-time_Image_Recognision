#include<iostream>
#include <opencv2/opencv.hpp>
#include <fcntl.h>  
#include <sys/stat.h>
#include <mqueue.h>
//#include <string>


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

int readLastNumber(char mtext[], int current, int lastSpace){
    if(current <= lastSpace)
        return -1;
    int lastCharacter = lastSpace+1;
    char help[current - lastCharacter];
    int i=0;

    //wrire part from original array into help array
    for( int between = lastCharacter; between < current; between++ ){
        help[i]=mtext[between];
        i++;
    }
    int result;
    //turn content of an array into int
    sscanf(help, "%d", &result);
    return result;
}

void readMessage(int *x, int *y, int *length, int *hight, char mtext[]){
    int current=0, currentResault=0;
    int results[4] = {-1,-1,-1,-1};
    int lastSpace = 0;

    while(( int(mtext[current]) != 0 ) && (current < ROZMIAR_KOMUNIKATU)){
        if( mtext[current] == ' ' ){
            results[currentResault] = readLastNumber(mtext, current, lastSpace);
            lastSpace = current;
            currentResault++;
        }
        current++;
    }
    results[currentResault] = readLastNumber(mtext, current, lastSpace);

    (*x) = results[0];
    (*y) = results[1];
    (*length) = results[2];
    (*hight) = results[3];
}

char findFieldCoordinateInDimention( int coordinate, int dimension ){
    int divide[3];
    divide[0]=dimension/3;
    divide[1]=dimension/3;
    divide[2]=dimension - divide[0] - divide[1];
    int rightBoarderA = divide[0], rightBoarderB = divide[0] + divide[1], rightBoarderC = dimension;
    char result;
    if( coordinate > rightBoarderC || coordinate < 0)
        std::cerr << "coordinate has wrong value" << std::endl;
    else if( coordinate <= rightBoarderC && coordinate >= rightBoarderB )
        result = 'C';
    else if( coordinate < rightBoarderB && coordinate >= rightBoarderA )
        result = 'B';
    else if( coordinate < rightBoarderA && coordinate >= 0 )
        result = 'A';
    else
        std::cerr << "coordinate has wrong value" << std::endl;
    return result;
}

void findField(int x, int y, int length, int height, char *fieldLength, char *fieldHeight){
    (*fieldLength) = findFieldCoordinateInDimention( x, length );
    char help = findFieldCoordinateInDimention( height - y, height );
    switch(help){
        case 'C': (*fieldHeight)='1'; break;
        case 'B': (*fieldHeight)='2'; break;
        case 'A': (*fieldHeight)='3'; break;
        defauld: std::cerr << "findFieldCoordinateInDimention returned wrong value" << std::endl;
    }
}

std::string format_message(char fieldLength, char fieldHeight) {
    std::string s = "";
    s.push_back(fieldLength);
    s.push_back(fieldHeight);
    return s;
}

int main(int argc, char** argvdd) {

    //otwarcie kolejki
    int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
    if(id_kolejki==-1){
        std::cout<<"Blad otwarcia kolejki\n";
        return 1;
    }
    bufmsg buf;
    int i = 0;

    while (true) {

        if (msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 3, 0) == -1) {
            std::cerr << "Error while receiving message from B" << std::endl;
            return 1;
        }

        std::cerr << "message from B received" << std::endl;
//        for(int j=0; j < ROZMIAR_KOMUNIKATU; j++){
//            std::cout << int(buf.mtext[j]) << " ";
//        }

        int x, y, length, height;
        readMessage(&x, &y, &length, &height, buf.mtext);

//        std::cerr << " x: " << x;
//        std::cerr << " y: " << y;
//        std::cerr << " length: " << length;
//        std::cerr << " height: " << height;

        char fieldLength, fieldHeight;
        findField(x, y, length, height, &fieldLength, &fieldHeight);

        buf.mtype = 4;
        strncpy(buf.mtext, format_message(fieldLength, fieldHeight).c_str(), ROZMIAR_KOMUNIKATU);

        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to D" << std::endl;
            return 1;
        }
        i++;
        if (i >= 6) break;
    }

}