#ifndef COIN_DISPENSER_H
#define COIN_DISPENSER_H

/*
  Deklaracja funkcji dispense_coins:
  - Próbuje wydać żądaną kwotę przy użyciu zasobów (5,2,1) * 10^scale.
  - Zwraca resztę (int), której nie da się wydać.
  - Przez *usedCoins i *usedCount przekazuje tablicę rodzaju wydanych żetonów.
*/

int dispense_coins(unsigned long amount,
                   int *supply5, int *supply2, int *supply1,
                   int scale,
                   int **usedCoins,
                   int *usedCount);

#endif
