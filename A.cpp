#include<iostream>
#include <opencv2/opencv.hpp>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <cstdio>


#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100

struct bufmsg{
	long mtype; /* 1 - z A do B; 2 - z B do A */
	char mtext[ROZMIAR_KOMUNIKATU];
};


int main() {

	cv::Mat frame;
	bufmsg buf;

	//otwarcie kolejki
	int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
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
		msgctl(id_kolejki, IPC_RMID, NULL);
		shmctl(id_pamieci, IPC_RMID, NULL);
		return 1;
	}

	int i = 0;//do usunięcia

	while(1){

		camera >> frame;

		//czy inny proces oczekuje na klatkę
		if(msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 2, IPC_NOWAIT)!=-1){
			//tu będzie wstawienie ramki do pamięci współdzielonej (otworzenie pamięci - shmat, modyfikacja pamięci, zamknięcie pamięci - shmdt)


	
			//wysylanie komunikatu
			buf.mtype = 1;
			sprintf(buf.mtext, "1");
			if(msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0)==-1){
				std::cout<<"Nie wyslano komunikatu - blad\n";
				return 1;
			}

			//musi być jakiś warunek zakończenia procesu (chyba), to jest bardzo chwilowy, przekaże 6 klatek i koniec


			++i;//do usunięcia
			if(i==6){//do usunięcia
				break;//do usunięcia
			}//do usunięcia
		}
	}
	return 0;
}
