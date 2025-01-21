#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include "UnitATM_tools.h"

int parse_coins(const char *coins_str, int *coins) {  //parsowanie ilosci monet
    return sscanf(coins_str, "%d:%d:%d", &coins[0], &coins[1], &coins[2]) == 3 ? 0 : -1;
}

void start_atm(int scale, int *coins, int descriptor, double delay, int seed) {
    int mult = 1;
    for (int i = 0; i < scale; i++){   //wyliczenie potegi 10
        mult *= 10;
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        //pomijanie bialych znakow
        int i = 0;
        while (isspace((unsigned char)buffer[i])) {
         i++;
        }
        if (buffer[i] == '\0') {  // 
            continue; 
        }


        //wyciagnie liczby z bufora
        char *endptr;
        long req_amount = strtol(buffer, &endptr, 10);

        if (endptr == buffer || *endptr != '\n' || req_amount < 0) {  //spradzenie czy liczba i czy jest dodatnia
            fprintf(stderr, "bledne dane\n");
            continue;
        }

        int left_amount = req_amount;
        int given_coins[3] = {0, 0, 0};  // wydane monety

        // wydawanie
        int values[3] = {5 * mult, 2 * mult, 1 * mult};
        for (int i = 0; i < 3; i++) {
            while (coins[i] > 0 && left_amount >= values[i]) {
                given_coins[i]++;
                coins[i]--;
                left_amount -= values[i];
            }
        }

        // niemozliwe do wydania
        printf("reszta: %d\n", left_amount);

        // tablica wydanych monet
        int all_coins = given_coins[0] + given_coins[1] + given_coins[2];
        int *disp_coins = (int *)malloc(all_coins * sizeof(int)); 

        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < given_coins[i]; j++) {
                disp_coins[index++] = values[i]; 
            }
        }

        srand(seed); //ziarno
        //losowy rozklad monet w tablicy do wypisania
        for (int i = all_coins - 1; i > 0; i--) {
            int j = rand() % (i + 1);  
            int temp = disp_coins[i];
            disp_coins[i] = disp_coins[j];
            disp_coins[j] = temp;
        }

        // wypisanie na deskryptor
        for (int i = 0; i < all_coins; i++) {
            dprintf(descriptor, "%d\n", disp_coins[i]);
            usleep((int)(delay * 10000));  //opoznienie centysekundy
        }
        dprintf(descriptor, "\n");

        free(disp_coins);
    }
}
