#include "UnitATM.h"
#include "coin_dispenser.h"

/*
  Funkcja dispense_coins:
  - Wykonuje algorytm zachłanny dla nominałów 5,2,1 (z uwzględnieniem 10^scale).
  - amount  – żądana kwota (już „bezpośrednio” w jednostkach, tzn. w readValue).
  - scale   – wykładnik potęgi 10 (0 => brak skali, 1 => x10, 2 => x100 itd.).
  - supply5, supply2, supply1 – zasoby poszczególnych monet w sztukach.
  - *usedCoins/*usedCount – tablica i liczba wydanych żetonów (1,2,5).
  - Zwraca resztę (int), której nie udało się wydać w tej samej skali co 'amount'.
*/
int dispense_coins(unsigned long amount,
                   int *supply5, int *supply2, int *supply1,
                   int scale,
                   int **usedCoins,
                   int *usedCount)
{
    // Wyliczamy 10^scale
    unsigned long scale_factor = 1UL;
    for(int i = 0; i < scale; i++) {
        scale_factor *= 10UL;
    }

    unsigned long denom5 = 5UL * scale_factor;
    unsigned long denom2 = 2UL * scale_factor;
    unsigned long denom1 = 1UL * scale_factor;

    // Algorytm zachłanny
    unsigned long canUse5 = amount / denom5;
    if (canUse5 > (unsigned long)(*supply5)) {
        canUse5 = *supply5;
    }
    unsigned long remainder = amount - canUse5 * denom5;

    unsigned long canUse2 = remainder / denom2;
    if (canUse2 > (unsigned long)(*supply2)) {
        canUse2 = *supply2;
    }
    remainder -= canUse2 * denom2;

    unsigned long canUse1 = remainder / denom1;
    if (canUse1 > (unsigned long)(*supply1)) {
        canUse1 = *supply1;
    }
    remainder -= canUse1 * denom1;

    // Aktualizujemy zasoby
    *supply5 -= canUse5;
    *supply2 -= canUse2;
    *supply1 -= canUse1;

    // Przygotowujemy tablicę użytych żetonów
    *usedCount = (int)(canUse5 + canUse2 + canUse1);
    *usedCoins = (int*) malloc((*usedCount) * sizeof(int));
    if (*usedCoins == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(1);
    }

    int idx = 0;
    for (unsigned long i = 0; i < canUse5; i++) {
        (*usedCoins)[idx++] = 5;
    }
    for (unsigned long i = 0; i < canUse2; i++) {
        (*usedCoins)[idx++] = 2;
    }
    for (unsigned long i = 0; i < canUse1; i++) {
        (*usedCoins)[idx++] = 1;
    }

    // Zwracamy resztę (int; tak jest w specyfikacji)
    return (int) remainder;
}
