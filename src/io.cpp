#include "io.h"

#include <fstream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <unordered_map>

std::string readTextFromFile(const std::string& filename) {
    // Czytamy cały plik 1:1 jako bajty (tryb binary = brak konwersji końców linii).
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku wejsciowego: " + filename);
    }

    return std::string(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
}

void writeTextToFile(const std::string& filename, const std::string& text) {
    // Zapisujemy dokładnie tyle bajtów, ile ma tekst (bez żadnych modyfikacji).
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna zapisac pliku: " + filename);
    }

    file.write(text.data(), static_cast<std::streamsize>(text.size()));
}

void writeCompressedFile(const std::string& filename,
                         const std::unordered_map<char, std::string>& codes,
                         const std::vector<uint8_t>& data,
                         uint32_t bitCount) {

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku wyjsciowego: " + filename);
    }

    // 1) Rozmiar słownika
    uint32_t dictSize = static_cast<uint32_t>(codes.size());
    file.write(reinterpret_cast<char*>(&dictSize), sizeof(dictSize));

    // 2) Wpisy słownika: znak + długość kodu + kod jako tekst '0'/'1'
    for (const auto& [ch, code] : codes) {
        file.write(&ch, sizeof(char));

        uint8_t codeLen = static_cast<uint8_t>(code.size());
        file.write(reinterpret_cast<char*>(&codeLen), sizeof(codeLen));

        file.write(code.data(), codeLen);
    }

    // 3) Liczba ważnych bitów (pozwala dekoderowi pominąć zera dopchane w ostatnim bajcie)
    file.write(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));

    // 4) Dane binarne (bajty)
    if (!data.empty()) {
        file.write(reinterpret_cast<const char*>(data.data()),
                   static_cast<std::streamsize>(data.size()));
    }
}

CompressedData readCompressedFile(const std::string& filename) {
    // Odczyt zgodnie z formatem z writeCompressedFile + walidacja (wyjątek, jeśli plik ucięty/uszkodzony).
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    CompressedData cd;

    // 1) dictSize
    uint32_t dictSize;
    file.read(reinterpret_cast<char*>(&dictSize), sizeof(dictSize));
    if (!file) throw std::runtime_error("Uszkodzony plik: brak dictSize");

    // 2) Słownik (czytamy do reverseDict: "101" -> 'a')
    for (uint32_t i = 0; i < dictSize; ++i) {
        char ch;
        uint8_t codeLen;

        file.read(&ch, sizeof(char));
        file.read(reinterpret_cast<char*>(&codeLen), sizeof(codeLen));
        if (!file) throw std::runtime_error("Uszkodzony plik: blad slownika");

        std::string code(codeLen, '0');
        file.read(code.data(), codeLen);
        if (!file) throw std::runtime_error("Uszkodzony plik: blad slownika");

        cd.reverseDict[code] = ch;
    }

    // 3) bitCount
    file.read(reinterpret_cast<char*>(&cd.bitCount), sizeof(cd.bitCount));
    if (!file) throw std::runtime_error("Uszkodzony plik: brak bitCount");

    // 4) Dane: liczba bajtów to zaokrąglenie w górę liczby bitów do bajtów
    size_t byteCount = (cd.bitCount + 7) / 8;
    cd.data.resize(byteCount);

    if (byteCount > 0) {
        file.read(reinterpret_cast<char*>(cd.data.data()),
                  static_cast<std::streamsize>(byteCount));
        if (!file) throw std::runtime_error("Uszkodzony plik: dane binarne");
    }

    return cd;
}