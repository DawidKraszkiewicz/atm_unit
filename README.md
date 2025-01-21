# Opis
Program unit_atm jest przeznaczony do wydawania żetonów o nominałach "5", "2" oraz "1", przemnożonych przez ustaloną skalę. Z założenia, skala jest nieujemną potęgą liczby 10.

Program pobiera ze standardowego wejścia liczby i wyznacza dla nich:
żetony, jakie ma wydać,
resztę, której się wydać nie da za pomocą posiadanych nominałów.
Wyniki są wysyłane do dwóch strumieni. Na standardowe wyjście jest wypisywana reszta, a opis wydawanych żetonów jest umieszczany w strumieniu o deskryptorze podanym w parametrach uruchomienia.
Każde wydanie żetonów uszczupla zasoby. 

Program działa tak długo, aż nie zostanie zamknięte standardowe wejście.


# Specyfikacja
Parametry uruchomienia:

-u <skala>  – liczba całkowita nieujemna, wykładnik potęgi skali;
  parametr opcjonalny, z wartością domyślną 0,\
-m <zasoby> – początkowe ilości poszczególnych monet; format wartości jest opisany niżej;
  parametr obowiązkowy,\
-t <wyniki> – numer deskryptora, na który mają być wypisywane informacje o żetonach;
  parametr obowiązkowy,\
-s <ziarno> – wartość inicjująca dla generatora liczb losowych (`random()`);
  parametr opcjonalny, z domyślną wartością równą PID,\
-d <opóźnienie> - liczba rzeczywista dodatnia, określająca odstęp czasu w centysekundach,
  parametr opcjonalny, z domyślną wartością 1.\
Wartość parametru -m (<zasoby>) jest łańcuchem składającym się z trzech liczb całkowitych, rozdzielanych dwukropkami.\ Poszczególne liczby opisują początkowe ilości monet o nominałach, odpowiednio "5", "2" oraz "1". Z tego powodu liczby te nie mogą być ujemne.

Wartość parametru -t jest deskryptorem, więc musi być liczbą całkowitą. Minimalny warunek, jaki musi ona spełniać, to być większą od zera.



# Format danych i wyników
Program odczytuje dane całymi wierszami. Aby wiersz był uznany za poprawny, musi zawierać wyłącznie jedną liczbę całkowitą nieujemną. Dozwolone są białe znaki przed i za nią.
Wiersze niespełniające warunku są ignorowane, ale z wypisaniem komunikatu na strumień diagnostyczny. Wyjątkiem są wiersze puste, oraz wypełnione wyłącznie białymi znakami. Te są ignorowane bez komentarzy.  

Informacja o reszcie, której nie można wydać, ma być wypisywana na standardowe wyjście, w formacie liczby dziesiętnej, z nową linią na końcu.

Informacje o wydawanych żetonach mają być wypisywane do strumienia o deskryptorze zadanym parametrem -t.
Dla każdego żetonu ma być to napis (wartość dziesiętnie), kończący się nową linią. Wartością żetonu jest jego nominał przemnożony przez skalę. Po wypisaniu informacji o wszystkich monetach, dodawany jest jeszcze jeden pusty wiersz.

# Uwaga:
1. między wypisywaniem wartości kolejnych żetonów ma być opóźnienie długości określonej parametrem -d,
2. monety mają być wypisywane w losowej kolejności.


# Sugestie
1. Dla poprawienia czytelności kodu źródłowego zaleca się go podzielić na kilka plików.
2. Główny program (podzielony na procedury) powinien znaleźć się w pliku UnitATM.c.
3. Procedury pomocnicze, w tym rozpoznawanie parametrów i weryfikacja ich poprawności, powinny znaleźć się w pliku UnitATM_tools.c (a ich deklaracje w UnitATM_tools.h).

# Kompilacja
```
make
```

lub

```
gcc -Wall -O2 -c UnitATM.c
gcc -Wall -O2 -c UnitATM_tools.c
gcc -Wall -O2 -c coin_dispenser.c
gcc -Wall -O2 -c random_tools.c
gcc -o unit_atm UnitATM.o UnitATM_tools.o coin_dispenser.o random_tools.o
```

# Przykład Użycia z wykorzystaniem minimalnej ilości parametrów
```
./unit_atm -m 3:4:10 -t 3 < wejscie.txt 3> wyjscie_zetony.txt
```

# Przykład Użycia z wykorzystaniem parametrów dodatkowych

```
./unit_atm -m 5:5:10 -t 3 -u 2 -s 999 -d 2.5 < input.txt 3> coins_output.txt
```

