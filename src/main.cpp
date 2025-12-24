#include <iostream>
#include <unordered_map>
#include <string>

#include "minheap.h"
#include "huffman.h"

bool compareHuffmanNodes(HuffmanNode* const& a, HuffmanNode* const& b) {
    return a->freq < b->freq;
}

std::unordered_map<char, int> countFrequencies(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }
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

int main() {
    std::string text = "ABRACADABRA";

    auto freq = countFrequencies(text);
    HuffmanNode* root = buildHuffmanTree(freq);

    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    std::cout << "Kody Huffmana:\n";
    for (const auto& [ch, code] : codes) {
        std::cout << ch << " -> " << code << "\n";
    }

    return 0;
}