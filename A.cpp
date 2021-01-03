#include<iostream>
#include <opencv2/opencv.hpp>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <cstdio>


#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346


struct bufmsg{
	long mtype; /* 1 - z A do B; 2 - z B do A */
	char mtext[2];
};


int main() {

	cv::Mat frame;
	bufmsg buf;

	//utworzenie lub otwarcie istniejącej kolejki
	int id_kolejki = msgget(KLUCZ_KOLEJKA, IPC_CREAT|0666);
	if(id_kolejki==-1){
		std::cout<<"Blad otwarcia kolejki\n";
		return 1;
	}

	//tworzenie pamięci współdzielonej
	int id_pamieci = shmget(KLUCZ_PAMIEC, sizeof(frame), IPC_CREAT|0666);
	if(id_pamieci==-1){
		std::cout<<"Blad utworzenia pamieci\n";
		msgctl(id_kolejki, IPC_RMID, NULL);
		return 1;
	}

	//utworzenie deskryptora kamery
    	cv::VideoCapture camera(0);
	if(!camera.isOpened()){
		std::cout<<"Nie otworzono kamere - blad\n";
		msgctl(id_kolejki, IPC_RMID, NULL);
		shmctl(id_pamieci, IPC_RMID, NULL);
		return 1;
	}

	int i = 0;
	while(1){
		camera >> frame;
		//czy inny proces oczekuje na klatkę
		if(msgrcv(id_kolejki, &buf, 2, 2, IPC_NOWAIT)!=-1){
			//tu będzie wstawienie ramki do pamięci współdzielonej (otworzenie pamięci, modyfikacja pamięci, zamknięcie pamięci	
			//wysylanie komunikatu
			buf.mtype = 1;
			sprintf(buf.mtext, "1");
			if(msgsnd(id_kolejki, &buf, 2, 0)==-1){
				std::cout<<"Nie wyslano komunikatu - blad\n";
				break;
			}
			++i;//do usunięcia
			if(i==6){//do usunięcia
				break;//do usunięcia
			}//do usunięcia
		}
	}

	//usunięcie kolejki
	msgctl(id_kolejki, IPC_RMID, NULL);
	//usunięcie pamięci
	shmctl(id_pamieci, IPC_RMID, NULL);
	return 0;
}
