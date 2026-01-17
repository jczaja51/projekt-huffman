#ifndef IO_H
#define IO_H

#include <string>
#include <unordered_map>
#include <vector>

// Dane potrzebne do dekompresji (wczytywane z pliku skompresowanego).
struct CompressedData {
    std::unordered_map<std::string, char> reverseDict; // "010" -> znak
    std::vector<uint8_t> data;                         // bajty strumienia bitów
    uint32_t bitCount;                                 // liczba ważnych bitów (bez paddingu)
};

// Czyta cały plik tekstowy (1:1 w trybie binary).
std::string readTextFromFile(const std::string& filename);

// Zapisuje tekst do pliku (1:1 w trybie binary).
void writeTextToFile(const std::string& filename, const std::string& text);

// Zapisuje plik skompresowany: słownik + bitCount + dane binarne.
void writeCompressedFile(const std::string& filename,
                         const std::unordered_map<char, std::string>& codes,
                         const std::vector<uint8_t>& data,
                         uint32_t bitCount);

// Wczytuje plik skompresowany do struktury CompressedData.
CompressedData readCompressedFile(const std::string& filename);

#endif