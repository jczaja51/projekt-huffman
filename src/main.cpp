#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

#include "minheap.h"
#include "huffman.h"

bool compareHuffmanNodes(HuffmanNode* const& a, HuffmanNode* const& b) {
    return a->freq < b->freq;
}

std::unordered_map<char, int> countFrequencies(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) freq[c]++;
    return freq;
}

HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    MinHeap<HuffmanNode*> heap(freq.size(), compareHuffmanNodes);
    for (const auto& [ch, f] : freq) {
        heap.insert(new HuffmanNode(ch, f));
    }
    while (heap.getSize() > 1) {
        HuffmanNode* left = heap.extractMin();
        HuffmanNode* right = heap.extractMin();
        heap.insert(new HuffmanNode(left, right));
    }
    return heap.extractMin();
}

std::string encodeText(const std::string& text,
                       const std::unordered_map<char, std::string>& codes) {
    std::ostringstream out;
    for (char c : text) out << codes.at(c);
    return out.str();
}

void writeCompressedFile(const std::string& filename,
                          const std::unordered_map<char, std::string>& codes,
                          const std::string& encodedBits) {
    std::ofstream file(filename);
    if (!file) throw std::runtime_error("Nie mozna otworzyc pliku wyjsciowego");

    file << "#HUFFMAN\n#DICT\n";
    for (const auto& [ch, code] : codes) {
        file << ch << ":" << code << "\n";
    }
    file << "#DATA\n" << encodedBits << "\n";
}

int main() {
    std::string text = "ABRACADABRA";

    auto freq = countFrequencies(text);
    HuffmanNode* root = buildHuffmanTree(freq);

    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    std::string encoded = encodeText(text, codes);

    writeCompressedFile("compressed.huf", codes, encoded);

    std::cout << "Zapisano plik compressed.huf\n";
    std::cout << "Dlugosc oryginalu: " << text.size() * 8 << " bitow\n";
    std::cout << "Dlugosc po kompresji: " << encoded.size() << " bitow\n";

    return 0;
}