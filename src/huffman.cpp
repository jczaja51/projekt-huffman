#include "huffman.h"
#include "io.h"
#include "minheap.h"

#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

bool compareHuffmanNodes(HuffmanNode* const& a, HuffmanNode* const& b) {
    return a->freq < b->freq;
}

static std::unordered_map<char, int> countFrequencies(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) freq[c]++;
    return freq;
}

static HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    if (freq.empty()) return nullptr;

    MinHeap<HuffmanNode*> heap((int)freq.size(), compareHuffmanNodes);

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

void generateCodes(HuffmanNode* node,
                   const std::string& prefix,
                   std::unordered_map<char, std::string>& codes) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[node->ch] = prefix.empty() ? "0" : prefix;
        return;
    }

    generateCodes(node->left,  prefix + "0", codes);
    generateCodes(node->right, prefix + "1", codes);
}

void compressFile(const std::string& inputFile,
                  const std::string& outputFile) {
    std::string text = readTextFromFile(inputFile);

    if (text.empty()) {
        std::unordered_map<char, std::string> empty;
        writeCompressedFile(outputFile, empty, "");
        std::cout << "Pusty plik wejsciowy. Zapisano: " << outputFile << "\n";
        return;
    }

    auto freq = countFrequencies(text);
    HuffmanNode* root = buildHuffmanTree(freq);

    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    std::ostringstream encoded;
    for (char c : text) {
        encoded << codes.at(c);
    }

    writeCompressedFile(outputFile, codes, encoded.str());

    std::cout << "OK: kompresja zakonczona\n";
    std::cout << "Oryginal: " << text.size() * 8 << " bitow\n";
    std::cout << "Po kompresji: " << encoded.str().size() << " bitow\n";
}

void decompressFile(const std::string& inputFile,
                    const std::string& outputFile) {
    CompressedData data = readCompressedFile(inputFile);

    std::string decoded;
    std::string buffer;

    for (char bit : data.bits) {
        buffer += bit;
        auto it = data.reverseDict.find(buffer);
        if (it != data.reverseDict.end()) {
            decoded += it->second;
            buffer.clear();
        }
    }

    if (!buffer.empty()) {
        throw std::runtime_error("Nie mozna w pelni zdekodowac danych");
    }

    writeTextToFile(outputFile, decoded);
    std::cout << "OK: dekompresja zakonczona\n";
}

struct DemoItem {
    std::string name;
    int priority;
};

static bool lessDemoItem(const DemoItem& a, const DemoItem& b) {
    return a.priority < b.priority;
}

void runHeapDemo() {
    std::cout << "=== HEAP DEMO ===\n";

    DemoItem items[] = {
        {"A", 5},
        {"B", 2},
        {"C", 8},
        {"D", 1},
        {"E", 3}
    };

    MinHeap<DemoItem> heap(10, lessDemoItem);
    heap.buildFromArray(items, 5);

    std::cout << "[BUILD FROM ARRAY]\n";
    heap.debugPrint([](const DemoItem& x) {
        std::cout << x.name << "(" << x.priority << ") ";
    });
    std::cout << "\n";

    std::cout << "\n[INSERT] X(4)\n";
    heap.insert({"X", 4});

    if (heap.getSize() > 1) {
        std::cout << "\n[DECREASE KEY] index=1, 4 -> 0\n";
        heap.decreaseKey(1, {"Z", 0});
    }

    std::cout << "\n[EXTRACT MIN]\n";
    while (!heap.isEmpty()) {
        DemoItem m = heap.extractMin();
        std::cout << m.name << "(" << m.priority << ")\n";
    }

    std::cout << "\n[ISEMPTY] -> " << (heap.isEmpty() ? "true" : "false") << "\n";
}