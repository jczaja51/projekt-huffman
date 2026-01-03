#include "io.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

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
        file << (int)(unsigned char)ch << ":" << code << "\n";
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
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line == "#DATA") break;
        if (line.empty()) continue;

        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string left = line.substr(0, pos);
        std::string right = line.substr(pos + 1);

        bool isNumber = !left.empty() &&
                        std::all_of(left.begin(), left.end(), ::isdigit);
        if (!isNumber) continue;

        int ascii = std::stoi(left);
        if (ascii < 0 || ascii > 255) continue;

        char ch = static_cast<char>(ascii);
        data.reverseDict[right] = ch;
    }

    std::ostringstream bits;
    bits << file.rdbuf();
    data.bits = bits.str();

    return data;
}