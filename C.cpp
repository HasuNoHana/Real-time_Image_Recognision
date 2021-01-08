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


//to co poniżej można by było przenieść do jakiegoś inego pliku .hpp (define oraz strukturę)
#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100
#define PODZIAL_DLUGOSC 3 //constant
#define PODZIAL_WYSOKOSC 3 //constant


struct bufmsg{
    long mtype; /* 1 - z A do B; 2 - z B do A; 3 - z B do C; 4 - z C do D */
    char mtext[ROZMIAR_KOMUNIKATU];
};

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

void readComunicate(int *x, int *y, int *length, int *hight, char mtext[]){
    int current=0, currentResault=0;
    int results[5] = {-1,-1,-1,-1,-1 };
    char num[ROZMIAR_KOMUNIKATU];
    int lastSpace = 0;

    while(( int(mtext[current]) != 0 ) && (current < ROZMIAR_KOMUNIKATU)){
//            std::cout << int(mtext[current]) << " ";
        if( mtext[current] == ' ' ){
            results[currentResault] = readLastNumber(mtext, current, lastSpace);
            lastSpace = current;
            currentResault++;
        }
        current++;
    }
    results[currentResault] = readLastNumber(mtext, current, lastSpace);

    (*x) = results[1];
    (*y) = results[2];
    (*length) = results[3];
    (*hight) = results[4];
}

char findFieldCoordinateInDimention( int coordinate, int dimension ){
    int divide[3];
    divide[0]=dimension/3;
    divide[1]=dimension/3;
    divide[2]=dimension - divide[0] - divide[1];
    int rightBoarderA = divide[0], rightBoarderB = divide[0] + divide[1], rightBoarderC = dimension;
    char result;
    std::cout << " dimension: " << dimension;
    std::cout << " coordinate: " << coordinate;
    std::cout << " rightBoarderA: " << rightBoarderA;
    std::cout << " rightBoarderB: " << rightBoarderB;
    std::cout << " rightBoarderC: " << rightBoarderC;
    if( coordinate > rightBoarderC || coordinate < 0)
        std::cerr << "coordinate has wrong value" << std::endl;
    else if( coordinate < rightBoarderC && coordinate > rightBoarderB )
        result = 'C';
    else if( coordinate < rightBoarderB && coordinate > rightBoarderA )
        result = 'B';
    else if( coordinate < rightBoarderA && coordinate > 0 )
        result = 'A';
    else
        std::cerr << "coordinate has wrong value" << std::endl;
    std::cout << " result: " << result << std::endl;
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

//        buf.mtext[0]=' ';
//        buf.mtext[1]='3';
//        buf.mtext[2]='3';
//        buf.mtext[3]='0';
//        buf.mtext[4]=' ';
//        buf.mtext[5]='1';
//        buf.mtext[6]='8';
//        buf.mtext[7]='0';
//        buf.mtext[8]=' ';
//        buf.mtext[9]='6';
//        buf.mtext[10]='4';
//        buf.mtext[11]='0';
//        buf.mtext[12]=' ';
//        buf.mtext[13]='4';
//        buf.mtext[14]='8';
//        buf.mtext[15]='0';
//        for(int j=16; j<50; j++ ){
//            buf.mtext[j]=0;
//        }//TODO delete this mock


        std::cerr << "message from B received" << std::endl;
//        for(int j=0; j < ROZMIAR_KOMUNIKATU; j++){
//            std::cout << int(buf.mtext[j]) << " ";
//        }

        int x, y, length, height;
        readComunicate(&x, &y, &length, &height, buf.mtext);

//        std::cout << " x: " << x;
//        std::cout << " y: " << y;
//        std::cout << " length: " << length;
//        std::cout << " height: " << height;

        char fieldLength, fieldHeight;
        findField(212, 159, length, height, &fieldLength, &fieldHeight);

        std::cout << " fieldLength: "<<fieldLength<<" fieldHeight: "<<fieldHeight<<std::endl;


        std::cout << std::endl << std::endl;

//        buf.mtype = 3;
//        strncpy(buf.mtext, format_message(x, y, dst.cols, dst.rows).c_str(), ROZMIAR_KOMUNIKATU);

//        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
//            std::cerr << "Error while sending message to C" << std::endl;
//            return 1;
//        }
        i++;
        if (i >= 6) break;
    }

}