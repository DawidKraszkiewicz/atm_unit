#include "UnitATM.h"
#include "random_tools.h"


void shuffle_array(int *array, int n)
{
    for(int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Zamiana elementów
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}
