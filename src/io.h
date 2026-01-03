#ifndef IO_H
#define IO_H

#include <string>
#include <unordered_map>

struct CompressedData {
    std::unordered_map<std::string, char> reverseDict;
    std::string bits;
};

std::string readTextFromFile(const std::string& filename);
void writeTextToFile(const std::string& filename, const std::string& text);

void writeCompressedFile(const std::string& filename,
                          const std::unordered_map<char, std::string>& codes,
                          const std::string& encodedBits);

CompressedData readCompressedFile(const std::string& filename);

#endif