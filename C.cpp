#include<iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/msg.h>


#include "dane.hpp"

int readLastNumber(char mtext[], int current, int lastSpace){
    if(current <= lastSpace)
        return -1;
    int lastCharacter = lastSpace+1;
    char help[current - lastCharacter];
    int i=0;

    //write part from original array into help array
    for( int between = lastCharacter; between < current; between++ ){
        help[i]=mtext[between];
        i++;
    }
    int result;
    //convert content of an array into int
    sscanf(help, "%d", &result);
    return result;
}

void readMessage(int *x, int *y, int *length, int *height, char mtext[]){
    int current=0, currentResult=0;
    int results[4] = {-1,-1,-1,-1};
    int lastSpace = 0;

    while((current < ROZMIAR_KOMUNIKATU) && ( int((mtext[current]) != 0 ))){
        if( mtext[current] == ' ' ){
            results[currentResult] = readLastNumber(mtext, current, lastSpace);
            lastSpace = current;
            currentResult++;
        }
        current++;
    }
    results[currentResult] = readLastNumber(mtext, current, lastSpace);

    (*x) = results[0];
    (*y) = results[1];
    (*length) = results[2];
    (*height) = results[3];
}

char findFieldCoordinateInDimention( int coordinate, int dimension ){
    int divide[3];
    divide[0]=dimension/3;
    divide[1]=dimension/3;
    divide[2]=dimension - divide[0] - divide[1];
    int rightBoarderA = divide[0], rightBoarderB = divide[0] + divide[1], rightBoarderC = dimension;
    char result;
    // if( coordinate > rightBoarderC || coordinate < 0)
    //     std::cout<< "coordinate has invalid value " << coordinate  << std::endl;
    /*else */if( coordinate <= rightBoarderC && coordinate >= rightBoarderB )
        result = 'C';
    else if( coordinate < rightBoarderB && coordinate >= rightBoarderA )
        result = 'B';
    else if( coordinate < rightBoarderA && coordinate >= 0 )
        result = 'A';
    // else
    //     std::cout << "coordinate has invalid value " << coordinate << std::endl;
    return result;
}

void findField(int x, int y, int length, int height, char *fieldLength, char *fieldHeight){
    (*fieldLength) = findFieldCoordinateInDimention( x, length );
    char help = findFieldCoordinateInDimention( height - y, height );
    switch(help){
        case 'C': (*fieldHeight)='1'; break;
        case 'B': (*fieldHeight)='2'; break;
        case 'A': (*fieldHeight)='3'; break;
        default: std::cerr << "findFieldCoordinateInDimention returned invalid value" << std::endl;
    }
}

std::string format_message(char fieldLength, char fieldHeight) {
    std::string s = "";
    s.push_back(fieldLength);
    s.push_back(fieldHeight);
    return s;
}

inline void error_message_exit(const char *message, uchar *shared_frame) {
    std::cerr << message << ": " << strerror(errno)  << std::endl;
    exit(1);
}

int main(int argc, char** argvdd) {

    //open queue
    int queue_id = msgget(KLUCZ_KOLEJKA_2, 0);
    if(queue_id==-1){
        error_message_exit("Error while opening message queue", nullptr);
    }
    bufmsg buf;
    bufmsg_txt buf_txt;
    int i = 0;

    while (true) {

        if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, -2, 0) == -1) {
            std::cerr << "Error while receiving message from B" << std::endl;
            return 1;
        }
        if (buf.mtype == 2) break;
        // std::cerr << "what\n";

        // std::cerr << buf.mtext[0] << " " << buf.mtext[1] << std::endl;
        // std::cerr << "B -> C" << std::endl;

        // std::cerr << "message from B received" << std::endl;
//        for(int j=0; j < ROZMIAR_KOMUNIKATU; j++){
//            std::cout << int(buf.mtext[j]) << " ";
//        }

        int x = buf.mtext[0];
        int y = buf.mtext[1];
        int length = buf.mtext[2];
        int height = buf.mtext[3];
        // readMessage(&x, &y, &length, &height, buf.mtext);

    //    std::cout << " x: " << x;
    //    std::cout << " y: " << y;
    //    std::cout << " length: " << length;
    //    std::cout << " height: " << height << std::endl;

        char fieldLength, fieldHeight;
        findField(x, y, length, height, &fieldLength, &fieldHeight);

        buf_txt.mtype = 3;
        strncpy(buf_txt.mtext, format_message(fieldLength, fieldHeight).c_str(), ROZMIAR_KOMUNIKATU);
        // std::cout << buf_txt.mtext << std::endl;
        if (msgsnd(queue_id, &buf_txt, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to D" << std::endl;
            return 1;
        }
        // i++;
        // if (i >= 1000) break;
    }
    std::cerr << "D -> C" << std::endl;
}