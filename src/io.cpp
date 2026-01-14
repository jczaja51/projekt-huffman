#include "io.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readTextFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku wejsciowego: " + filename);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeTextToFile(const std::string& filename, const std::string& text) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mozna zapisac pliku: " + filename);
    }
    file << text;
}

void writeCompressedFile(const std::string& filename,
                          const std::unordered_map<char, std::string>& codes,
                          const std::string& encodedBits) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku wyjsciowego: " + filename);
    }

    file << "#HUFFMAN\n";
    file << "#DICT\n";

    for (const auto& [ch, code] : codes) {
        if (ch == '\n')
            file << "\\n:" << code << "\n";
        else if (ch == ' ')
            file << " :" << code << "\n";
        else
            file << ch << ":" << code << "\n";
    }

    file << "#DATA\n";
    file << encodedBits;
}

CompressedData readCompressedFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    CompressedData data;
    std::string line;

    std::getline(file, line);
    if (line != "#HUFFMAN")
        throw std::runtime_error("Niepoprawny format pliku");

    std::getline(file, line);
    if (line != "#DICT")
        throw std::runtime_error("Brak sekcji #DICT");

    while (std::getline(file, line)) {
        if (line == "#DATA") break;
        if (line.empty()) continue;

        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string symbol = line.substr(0, pos);
        std::string code   = line.substr(pos + 1);

        char ch;
        if (symbol == "\\n")
            ch = '\n';
        else if (symbol == " ")
            ch = ' ';
        else
            ch = symbol[0];

        data.reverseDict[code] = ch;
    }

    std::ostringstream bits;
    bits << file.rdbuf();
    data.bits = bits.str();

    return data;
}