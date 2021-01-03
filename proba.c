#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>


#define KLUCZ 12345


int main(void) {
	int id_kolejki = msgget(KLUCZ, IPC_CREAT|0600);
	printf("OK\n");
	return 0;
}
