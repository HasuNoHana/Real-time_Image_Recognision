#include<iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/msg.h>

#include "dane.hpp"


char findFieldCoordinateInDimension( int coordinate, int dimension ){
    int divide[3];
    divide[0]=dimension/3;
    divide[1]=dimension/3;
    divide[2]=dimension - divide[0] - divide[1];
    int rightBorderA = divide[0], rightBorderB = divide[0] + divide[1], rightBorderC = dimension;
    char result;
    if( coordinate > rightBorderC || coordinate < 0)
         std::cout<< "coordinate has invalid value " << coordinate  << std::endl;
    else if( coordinate <= rightBorderC && coordinate >= rightBorderB )
        result = 'C';
    else if( coordinate < rightBorderB && coordinate >= rightBorderA )
        result = 'B';
    else if( coordinate < rightBorderA && coordinate >= 0 )
        result = 'A';
    else
        std::cout << "coordinate has invalid value " << coordinate << std::endl;
    return result;
}

void findField(int x, int y, int length, int height, char *fieldLength, char *fieldHeight){
    (*fieldLength) = findFieldCoordinateInDimension( x, length );
    char help = findFieldCoordinateInDimension( height - y, height );
    switch(help){
        case 'C': (*fieldHeight)='1'; break;
        case 'B': (*fieldHeight)='2'; break;
        case 'A': (*fieldHeight)='3'; break;
        default: std::cerr << "findFieldCoordinateInDimension returned invalid value" << std::endl;
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

    while (true) {

        if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, -2, 0) == -1) {
            std::cerr << "Error while receiving message from B" << std::endl;
            return 1;
        }
        if (buf.mtype == 2) break;

        int x = buf.mtext[0];
        int y = buf.mtext[1];
        int length = buf.mtext[2];
        int height = buf.mtext[3];

        char fieldLength, fieldHeight;
        findField(x, y, length, height, &fieldLength, &fieldHeight);

        buf_txt.mtype = 3;
        strncpy(buf_txt.mtext, format_message(fieldLength, fieldHeight).c_str(), ROZMIAR_KOMUNIKATU);
        
        if (msgsnd(queue_id, &buf_txt, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to D" << std::endl;
            return 1;
        }
    }
}