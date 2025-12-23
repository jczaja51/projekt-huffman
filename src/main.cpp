#include <iostream>
#include <unordered_map>
#include <string>

#include "minheap.h"
#include "huffman.h"

std::unordered_map<char, int> countFrequencies(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }
    return freq;
}

HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    struct HeapItem {
        HuffmanNode* node;
        int priority;
    };

    MinHeap heap(freq.size());

    std::vector<HuffmanNode*> nodes;

    // Wstaw liście
    for (const auto& [ch, f] : freq) {
        HuffmanNode* node = new HuffmanNode(ch, f);
        nodes.push_back(node);
        heap.insert({ch, f});
    }

    return nodes.front(); // placeholder
}

int main(int argc, char* argv[]) {

    // TESTOWY TEKST
    std::string text = "ABRACADABRA";

    // Liczenie częstotliwości
    auto freq = countFrequencies(text);

    std::cout << "Czestotliwosci znakow:\n";
    for (const auto& [ch, f] : freq) {
        std::cout << ch << " : " << f << std::endl;
    }

    // Budowa drzewa Huffmana
    HuffmanNode* root = buildHuffmanTree(freq);

    // Generowanie kodów
    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    // Wypisanie slownika
    std::cout << "\nKody Huffmana:\n";
    for (const auto& [ch, code] : codes) {
        std::cout << ch << " -> " << code << std::endl;
    }

    return 0;
}