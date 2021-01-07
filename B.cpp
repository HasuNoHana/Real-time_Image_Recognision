/*#include <stdio.h>
#include<iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv ) {
    std::cout << "hello world says B" << std::endl;
}*/

#include<iostream>
#include <opencv2/opencv.hpp>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>


#include "dane.hpp"


int main() {
	
	uchar* shared_frame;
	bufmsg buf;

	//otwarcie kolejki
	int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
	if(id_kolejki==-1){
		std::cout<<"Blad otwarcia kolejki\n";
		return 1;
	}

	//otwarcie pamięci współdzielonej
	int id_pamieci = shmget(KLUCZ_PAMIEC, ROZMIAR_PAMIECI, 0);
	if(id_pamieci==-1){
		std::cout<<"Blad otwarcia pamieci\n";
		return 1;
	}

	//otwarcie bufora pamięci współdzielonej
	shared_frame = (uchar*) shmat(id_pamieci, 0, 0);
	if(shared_frame==(uchar*)-1){
		std::cout<<"Blad otwarcia bufora pamieci\n";
		return 1;
	}

	buf.mtype = 2;
	sprintf(buf.mtext, "1");
	if(msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0)==-1){
		std::cout<<"Nie wyslano komunikatu - blad\n";
	
		//zamkniecie bufora pamieci
		if(shmdt(shared_frame)==-1){
			std::cout<<"Blad zamkniecia bufora pamieci\n";
		}
		return 1;
	}

	while(1){
		if(msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 1, 0)!=-1){			

			cv::Mat frame(cv::Size(640, 480), CV_8UC3, shared_frame,cv::Mat::AUTO_STEP);

			cv::namedWindow("Webcam", CV_WINDOW_AUTOSIZE);
			while(1){
				cv::imshow("Webcam", frame);
				if(cv::waitKey(10) >= 0)
					break;
			}
			break;
		}
}

	//zamkniecie bufora pamieci
	if(shmdt(shared_frame)==-1){
		std::cout<<"Blad zamkniecia bufora pamieci\n";
	}

	return 0;
}
