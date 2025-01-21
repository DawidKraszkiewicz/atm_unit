CC = gcc
CFLAGS = -Wall -O2

OBJS = UnitATM.o UnitATM_tools.o coin_dispenser.o random_tools.o

all: unit_atm

unit_atm: $(OBJS)
	$(CC) $(CFLAGS) -o unit_atm $(OBJS)

UnitATM.o: UnitATM.c UnitATM.h UnitATM_tools.h coin_dispenser.h random_tools.h
	$(CC) $(CFLAGS) -c UnitATM.c

UnitATM_tools.o: UnitATM_tools.c UnitATM.h UnitATM_tools.h
	$(CC) $(CFLAGS) -c UnitATM_tools.c

coin_dispenser.o: coin_dispenser.c UnitATM.h coin_dispenser.h
	$(CC) $(CFLAGS) -c coin_dispenser.c

random_tools.o: random_tools.c UnitATM.h random_tools.h
	$(CC) $(CFLAGS) -c random_tools.c

clean:
	rm -f *.o unit_atm
