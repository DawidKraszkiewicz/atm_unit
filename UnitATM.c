#include "UnitATM.h"
#include "UnitATM_tools.h"
#include "coin_dispenser.h"
#include "random_tools.h"

/*
  Główny program:
  - Parsuje argumenty (parse_arguments).
  - Inicjuje generator liczb pseudolosowych (srand).
  - Otwiera deskryptor podany przez -t (fdopen).
  - W pętli czyta linie ze stdin, ignoruje puste/niepoprawne.
  - Dla poprawnych liczb wywołuje dispense_coins, wypisuje resztę na stdout
    i wypisuje żetony (w losowej kolejności) na deskryptor -t, z opóźnieniem.
  - Kończy po EOF na standardowym wejściu.
*/

int main(int argc, char *argv[])
{
    int scale;
    int supply5, supply2, supply1;
    int t_descriptor;
    unsigned int seed;
    double delay_cs;

    // Parsowanie argumentów
    if (parse_arguments(argc, argv,
                        &scale,
                        &supply5, &supply2, &supply1,
                        &t_descriptor,
                        &seed,
                        &delay_cs) != 0)
    {
        // Błąd – wypisujemy usage i kończymy
        print_usage(argv[0]);
        return 1;
    }

    // Inicjujemy generator liczb pseudolosowych
    srand(seed);

    // Przeliczamy opóźnienie w centysekundach → mikrosekundy
    // 1 cs = 10_000 µs
    int delay_us = (int)(delay_cs * 10000.0);

    // Otwieramy podany deskryptor jako strumień wyjściowy
    FILE *token_output = fdopen(t_descriptor, "w");
    if (!token_output) {
        fprintf(stderr, "Error: cannot open descriptor %d for writing.\n",
                t_descriptor);
        return 1;
    }

    char line[256];
    while (1) {
        if (!fgets(line, sizeof(line), stdin)) {
            // EOF lub błąd czytania
            break;
        }

        // Pomijamy wiodące spacje
        char *p = line;
        while (isspace((unsigned char)*p)) p++;

        // Sprawdzamy, czy wiersz jest pusty (tylko białe znaki)
        if (*p == '\0') {
            // Pusta linia – ignorujemy bez komunikatu
            continue;
        }

        // Próba parsowania liczby całkowitej (long) nieujemnej
        errno = 0;
        char *endptr = NULL;
        long val = strtol(p, &endptr, 10);

        if ((errno != 0) || (endptr == p)) {
            // Nie udało się sparsować
            fprintf(stderr, "Ignored invalid input line: %s", line);
            continue;
        }
        // Sprawdzamy, czy za liczbą nie ma nic poza białymi znakami
        while (isspace((unsigned char)*endptr)) endptr++;
        if (*endptr != '\0' && *endptr != '\n') {
            // Niejednoznaczny wiersz
            fprintf(stderr, "Ignored invalid input line: %s", line);
            continue;
        }
        if (val < 0) {
            // Ujemna wartość – ignorujemy z komunikatem
            fprintf(stderr, "Ignored negative value: %ld\n", val);
            continue;
        }

        // Mamy poprawną wartość nieujemną
        unsigned long amount = (unsigned long) val;

        // Wywołujemy logikę wydawania żetonów
        int *usedCoins = NULL;
        int usedCount = 0;
        int remainder = dispense_coins(amount,
                                       &supply5, &supply2, &supply1,
                                       scale,
                                       &usedCoins, &usedCount);

        // Resztę wypisujemy na standardowe wyjście
        printf("%d\n", remainder);
        fflush(stdout);

        // Jeśli cokolwiek zostało wydane, wypisujemy żetony
        if (usedCount > 0) {
            // Przetasuj kolejność użytych żetonów
            shuffle_array(usedCoins, usedCount);

            // Obliczamy mnożnik 10^scale
            unsigned long scale_factor = 1UL;
            for (int i = 0; i < scale; i++) {
                scale_factor *= 10UL;
            }

            // Wypisujemy rodzaj żetonu (nominał * scale_factor) w nowej linii
            for (int i = 0; i < usedCount; i++) {
                unsigned long coinValue = (unsigned long)usedCoins[i] * scale_factor;
                fprintf(token_output, "%lu\n", coinValue);
                fflush(token_output);

                // Opóźnienie między kolejnymi żetonami
                usleep(delay_us);
            }

            // Po wszystkich żetonach – pusty wiersz
            fprintf(token_output, "\n");
            fflush(token_output);
        }

        free(usedCoins);
    }

    fclose(token_output);
    return 0;
}
