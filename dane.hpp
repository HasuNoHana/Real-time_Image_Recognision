#ifndef DANE_H
#define DANE_H

#define KLUCZ_KOLEJKA 12375
#define KLUCZ_PAMIEC 14567

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 921600


struct bufmsg{
    long mtype; /* 1 - z A do B; 2 - z B do A; 3 - z B do C; 4 - z C do D */
    char mtext[ROZMIAR_KOMUNIKATU];
};

#endif //DANE_H
