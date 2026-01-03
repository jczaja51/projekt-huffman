# Kodowanie Huffmana – kompresja i dekompresja plików (C++)

## 1. Opis projektu

Projekt realizuje **kompresję i dekompresję plików tekstowych** z wykorzystaniem **algorytmu kodowania Huffmana**.

Celem projektu jest praktyczne zastosowanie struktur danych i algorytmów omawianych na wykładzie, w szczególności:
- kolejki priorytetowej typu minimum (kopca),
- algorytmu Huffmana,
- operacji wejścia/wyjścia na plikach,
- obsługi użytkownika w trybie interaktywnym oraz z linii poleceń.

Program został napisany w języku **C++ (standard C++17)** i działa jako **aplikacja konsolowa**.  
Całość została zaprojektowana w sposób **modułowy**, bez przeciążania pliku `main.cpp`.

---

## 2. Zrealizowane funkcjonalności

Projekt obejmuje:
- własną implementację **kolejki priorytetowej typu minimum**,
- operacje na kolejce:
  - budowanie z tablicy,
  - dodawanie elementu,
  - usuwanie elementu minimalnego,
  - sprawdzanie, czy kolejka jest pusta,
  - zmniejszanie priorytetu elementu,
- demonstrację działania kolejki priorytetowej,
- kompresję plików tekstowych metodą Huffmana,
- dekompresję plików `.huf`,
- zapis słownika i danych w formacie omawianym na wykładzie,
- pełną obsługę błędów i walidację danych wejściowych.

---

## 3. Struktura projektu

Projekt składa się z następujących modułów:

### `minheap.*`
Implementacja kolejki priorytetowej (kopca minimalnego) jako struktury tablicowej.

### `huffman.*`
Logika algorytmu Huffmana:
- zliczanie częstotliwości znaków,
- budowa drzewa Huffmana,
- generowanie kodów,
- kompresja i dekompresja danych.

### `io.*`
Operacje wejścia/wyjścia:
- wczytywanie danych z plików,
- zapis plików skompresowanych (`.huf`),
- zapis plików zdekompresowanych (`.txt`).

### `main.cpp`
Obsługa użytkownika, menu, walidacja danych, wybór trybu pracy.

---

## 4. Wymagania systemowe

Do uruchomienia programu wymagane są:
- system operacyjny: **Windows / Linux / macOS**,
- kompilator C++ zgodny z **C++17** (`gcc`, `g++`, `clang`),
- **CMake w wersji co najmniej 3.16**,
- terminal (wiersz poleceń).

Program nie wymaga zewnętrznych bibliotek.

---

## 5. Budowanie projektu

Aby zbudować projekt, wykonaj poniższe kroki w terminalu:

```bash
# przejście do katalogu projektu
cd projekt-aisd

# utworzenie katalogu build i wejście do niego
mkdir build
cd build

# konfiguracja projektu
cmake ..

# kompilacja
cmake --build .
```
Po poprawnej kompilacji powstaje plik wykonywalny:
- `projekt-aisd` (Linux / macOS),
- `projekt-aisd.exe` (Windows).

---

## 6. Uruchamianie programu

Program można uruchomić na dwa sposoby.

### 6.1 Tryb interaktywny (menu)

Uruchom program bez argumentów:

```bash
./projekt-aisd
```
Wyświetlone zostanie menu:
```bash
=== Kodowanie Huffmana ===
1. Kompresja pliku
2. Dekompresja pliku
3. Demo kolejki priorytetowej
0. Wyjście
```
#### Kompresja pliku
1. Wybierz opcję `1`
2. Podaj nazwę pliku wejściowego (`.txt`)
3. Jeśli plik nie istnieje, program zapyta, czy go utworzyć
4. Podaj nazwę pliku wyjściowego (`.huf`)
5. Program wykona kompresję i wyświetli statystyki

#### Dekompresja pliku
1. Wybierz opcję `2`
2. Podaj nazwę pliku `.huf`
3. Podaj nazwę pliku wynikowego `.txt`
4. Program odtworzy oryginalny tekst

#### Demo kolejki priorytetowej
1. Wybierz opcję `3`
2. Program zaprezentuje:
   - budowę kopca z tablicy,
   - dodawanie elementu,
   - zmianę priorytetu,
   - usuwanie elementów,
   - sprawdzenie, czy kolejka jest pusta.

---

### 6.2 Tryb linii poleceń (CLI)

Program obsługuje również argumenty:

#### Kompresja
```bash
./projekt-aisd compress input.txt output.huf
```
#### Dekompresja
```bash
./projekt-aisd decompress input.huf output.txt
```
#### Demo kolejki
```bash
./projekt-aisd heap-demo
```
## 7. Format pliku skompresowanego `.huf`

Plik wynikowy ma format tekstowy:
#HUFFMAN
#DICT
A:0110
:110
a:10
#DATA
011011111011001010...


- `#DICT` – słownik kodów Huffmana (znak → kod binarny),
- `#DATA` – zakodowany strumień bitów.


---

## 8. Zabezpieczenia i walidacja

Program zawiera:
- sprawdzanie istnienia plików wejściowych,
- możliwość utworzenia pliku wejściowego z poziomu programu,
- automatyczne uzupełnianie rozszerzeń `.txt` i `.huf`,
- obsługę błędnych wyborów menu,
- obsługę pustych plików,
- walidację formatu pliku `.huf`,
- czytelne komunikaty błędów zamiast awarii programu.

