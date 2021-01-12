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

	cv::Mat frame;
	uchar* shared_frame;
	bufmsg_txt buf;

	//otwarcie kolejki
	int id_kolejki = msgget(KLUCZ_KOLEJKA_1, 0);
	if(id_kolejki==-1){
		std::cout<<"Blad otwarcia kolejki\n";
		return 1;
	}

	//otwarcie pamięci współdzielonej
	int id_pamieci = shmget(KLUCZ_PAMIEC, 0, 0);
	if(id_pamieci==-1){
		std::cout<<"Blad otwarcia pamieci\n";
		return 1;
	}

	//utworzenie deskryptora kamery
    	cv::VideoCapture camera(0);
	if(!camera.isOpened()){
		std::cout<<"Nie otworzono kamere - blad\n";
		return 1;
	}
	camera.set(cv::CAP_PROP_FPS, 10);

	//otwarcie bufora pamięci współdzielonej
	shared_frame = (uchar*) shmat(id_pamieci, 0, 0);
	if(shared_frame==(uchar*)-1){
		std::cout<<"Blad otwarcia bufora pamieci\n";
		return 1;
	}

	while(true){

		camera >> frame;

		//czy inny proces oczekuje na klatkę
		if(msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 3, IPC_NOWAIT)!=-1){
			// std::cerr << "B -> A" << std::endl;
			
			//wstawienie ramki do pamięci współdzielonej
			memcpy(shared_frame, frame.data, ROZMIAR_PAMIECI);

			//wysylanie komunikatu
			buf.mtype = 1;
			sprintf(buf.mtext, "1");
			if(msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0)==-1){
				std::cout<<"Nie wyslano komunikatu - blad\n";
				break;
			}
        }
		if(msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 4, IPC_NOWAIT)!=-1) break;
	}
	//zamkniecie bufora pamieci
	if(shmdt(shared_frame)==-1){
		std::cout<<"Blad zamkniecia bufora pamieci\n";
	}
	//zamkniecie kamery
	camera.release();

	return 0;
}
