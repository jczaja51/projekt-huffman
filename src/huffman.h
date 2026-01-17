#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

/*
  Węzeł drzewa Huffmana:
  - liść: przechowuje znak (ch) i jego częstotliwość (freq)
  - węzeł wewnętrzny: ch = '\0', freq = suma freq dzieci
*/
struct HuffmanNode {
    char ch;            // znak (sensowny tylko dla liści)
    int freq;           // częstotliwość znaku / suma częstotliwości w poddrzewie
    HuffmanNode* left;  // dziecko lewe (0)
    HuffmanNode* right; // dziecko prawe (1)

    // Konstruktor liścia: reprezentuje pojedynczy znak.
    HuffmanNode(char c, int f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // Konstruktor węzła wewnętrznego: łączy dwa poddrzewa.
    HuffmanNode(HuffmanNode* l, HuffmanNode* r)
        : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

// Kompresuje plik tekstowy do formatu Huffmana (słownik + dane + bitCount).
void compressFile(const std::string& inputFile,
                  const std::string& outputFile);

// Dekompresuje plik zapisany w formacie Huffmana do tekstu.
void decompressFile(const std::string& inputFile,
                    const std::string& outputFile);

// Krótka demonstracja działania MinHeap (nie jest częścią Huffmana).
void runHeapDemo();

#endif