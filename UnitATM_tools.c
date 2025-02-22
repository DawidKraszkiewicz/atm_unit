#include "UnitATM.h"
#include "UnitATM_tools.h"
#include <getopt.h> 

/*
  Funkcja parse_arguments:
  - Ustawia wartości domyślne dla parametrów.
  - Rozpoznaje argumenty wywołania.
*/
int parse_arguments(int argc, char *argv[],
                    int *scale,
                    int *supply5, int *supply2, int *supply1,
                    int *t_descriptor,
                    unsigned int *seed,
                    double *delay_cs)
{
    // Ustawiamy wartości domyślne
    *scale = 0;                      
    *seed = (unsigned int)getpid();  
    *delay_cs = 1.0;                 
    // Do sprawdzania czy parametry były podane
    int m_parsed = 0;  
    int t_parsed = 0;  

    int opt;
  
    while ((opt = getopt(argc, argv, "u:m:t:s:d:")) != -1) {
        switch (opt) {
            case 'u': {
                char *endptr = NULL;
                errno = 0;
                long tmp = strtol(optarg, &endptr, 10);
                if ((errno != 0) || (endptr == optarg) || (tmp < 0)) {
                    fprintf(stderr, "Error: invalid (non-negative) scale for -u.\n");
                    return -1;
                }
                *scale = (int)tmp;
                break;
            }

            case 'm': {
                // Zasoby w formacie x:y:z, każde >= 0
                int x, y, z;
                if (sscanf(optarg, "%d:%d:%d", &x, &y, &z) != 3) {
                    fprintf(stderr, "Error: invalid format for -m, expected x:y:z.\n");
                    return -1;
                }
                if (x < 0 || y < 0 || z < 0) {
                    fprintf(stderr, "Error: supplies cannot be negative.\n");
                    return -1;
                }
                *supply5 = x;
                *supply2 = y;
                *supply1 = z;
                m_parsed = 1;
                break;
            }

            case 't': {
                char *endptr = NULL;
                errno = 0;
                long tmp = strtol(optarg, &endptr, 10);
                if ((errno != 0) || (endptr == optarg) || (tmp <= 0)) {
                    fprintf(stderr, "Error: descriptor (-t) must be > 0.\n");
                    return -1;
                }
                *t_descriptor = (int)tmp;
                t_parsed = 1;
                break;
            }

            case 's': {
                char *endptr = NULL;
                errno = 0;
                long tmp = strtol(optarg, &endptr, 10);
                // Jeśli ujemne – uznajemy za błąd
                if ((errno != 0) || (endptr == optarg) || (tmp < 0)) {
                    fprintf(stderr, "Error: invalid (non-negative) seed for -s.\n");
                    return -1;
                }
                *seed = (unsigned int)tmp;
                break;
            }

            case 'd': {
                char *endptr = NULL;
                errno = 0;
                double tmp = strtod(optarg, &endptr);
                if ((errno != 0) || (endptr == optarg) || (tmp <= 0)) {
                    fprintf(stderr, "Error: invalid (positive) delay for -d.\n");
                    return -1;
                }
                *delay_cs = tmp;
                break;
            }
          
            default:
                return -1;
        }
    }

    // Po przejściu wszystkich opcji sprawdzamy, czy wymagane parametry są ok
    if (!m_parsed) {
        fprintf(stderr, "Error: -m parameter is mandatory.\n");
        return -1;
    }
    if (!t_parsed) {
        fprintf(stderr, "Error: -t parameter is mandatory.\n");
        return -1;
    }

    return 0;
}


void print_usage(const char *progname)
{
    fprintf(stdout, "Usage: %s -m x:y:z -t descriptor [options]\n", progname);
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "  -u <scale>  : non-negative integer exponent (default=0)\n");
    fprintf(stdout, "  -s <seed>   : random seed (non-negative, default=PID)\n");
    fprintf(stdout, "  -d <delay>  : delay in centiseconds, must be > 0 (default=1.0)\n");
}
