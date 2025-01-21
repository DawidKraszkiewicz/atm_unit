#ifndef UNITATM_TOOLS_H
#define UNITATM_TOOLS_H

/*
  Deklaracje funkcji pomocniczych do parsowania argumentów
  oraz wypisywania pomocy.
*/

int parse_arguments(int argc, char *argv[],
                    int *scale,
                    int *supply5, int *supply2, int *supply1,
                    int *t_descriptor,
                    unsigned int *seed,
                    double *delay_cs);

void print_usage(const char *progname);

#endif
