#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <string>

struct HuffmanNode {
    char ch;                 // znak (tylko dla liści)
    int freq;                // częstotliwość
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

#endif