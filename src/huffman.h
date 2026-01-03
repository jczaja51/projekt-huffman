#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    HuffmanNode(HuffmanNode* l, HuffmanNode* r)
        : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

void generateCodes(HuffmanNode* node,
                   const std::string& prefix,
                   std::unordered_map<char, std::string>& codes);

void compressFile(const std::string& inputFile,
                  const std::string& outputFile);

void decompressFile(const std::string& inputFile,
                    const std::string& outputFile);

void runHeapDemo();

#endif