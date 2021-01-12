#ifndef DANE_H
#define DANE_H

#define KLUCZ_KOLEJKA_1 12375
#define KLUCZ_KOLEJKA_2 12376

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

#endif //DANE_H
