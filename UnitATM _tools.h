#ifndef UNITATM_TOOLS_H
#define UNITATM_TOOLS_H

int parse_coins(const char *coins_str, int *coins);
void start_atm(int scale, int *coins, int descriptor, double delay, int seed);

#endif
