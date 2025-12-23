#include "huffman.h"

void generateCodes(HuffmanNode* node,
                   const std::string& prefix,
                   std::unordered_map<char, std::string>& codes) {
    if (!node) return;

    // liść
    if (!node->left && !node->right) {
        codes[node->ch] = prefix;
        return;
    }

    generateCodes(node->left,  prefix + "0", codes);
    generateCodes(node->right, prefix + "1", codes);
}
