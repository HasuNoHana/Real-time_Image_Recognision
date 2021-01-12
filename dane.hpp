#ifndef DANE_H
#define DANE_H

#define KLUCZ_KOLEJKA_1 12375
#define KLUCZ_KOLEJKA_2 12380

#define KLUCZ_PAMIEC 14567

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 921600


struct bufmsg{
    long mtype;
    // kolejka 1: A->B - 1; D->B - 2; B->A - 3; D->A - 4
    // kolejka 2: B->C - 1; D->C - 2; C->D - 3
    int mtext[ROZMIAR_KOMUNIKATU];
};
struct bufmsg_txt{
    long mtype;
    // kolejka 1: A->B - 1; D->B - 2; B->A - 3; D->A - 4
    // kolejka 2: B->C - 1; D->C - 2; C->D - 3
    char mtext[ROZMIAR_KOMUNIKATU];
};
// const int KLUCZ_KOLEJKA_1  = ftok("/home/aniela/Documents/queues/queue1",1);
// const int KLUCZ_KOLEJKA_2  = ftok("/home/aniela/Documents/queues/queue2",1);

#endif //DANE_H
