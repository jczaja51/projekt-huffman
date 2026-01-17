#include "huffman.h"
#include "io.h"
#include "minheap.h"

#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>

/*
    Comparator do MinHeap:
    Chcemy kopiec minimalny po częstotliwości, więc "mniejsze freq" ma wyższy priorytet.
*/
bool compareHuffmanNodes(HuffmanNode* const& a, HuffmanNode* const& b) {
    return a->freq < b->freq;
}

/*
    Krok 1 (Huffman): zliczamy ile razy występuje każdy znak.
    To na tej podstawie budujemy drzewo (znaki częstsze -> krótsze kody).
*/
static std::unordered_map<char, int> countFrequencies(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) freq[c]++;
    return freq;
}

/*
    Krok 2 (Huffman): budowa drzewa z użyciem kopca minimalnego.
    Idea: zawsze łączymy dwa NAJRZADSZE symbole/węzły w nowy węzeł-rodzic.
    Powtarzamy aż zostanie jeden węzeł (korzeń drzewa).
*/
static HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    if (freq.empty()) return nullptr;

    // Tworzymy liście dla każdego znaku.
    std::vector<HuffmanNode*> nodes;
    for (const auto& [ch, f] : freq) {
        nodes.push_back(new HuffmanNode(ch, f));
    }

    // MinHeap pozwala szybko brać dwa najmniejsze elementy (extractMin).
    MinHeap<HuffmanNode*> heap(nodes.size(), compareHuffmanNodes);
    heap.buildFromArray(nodes.data(), nodes.size());

    while (heap.getSize() > 1) {
        HuffmanNode* left  = heap.extractMin();
        HuffmanNode* right = heap.extractMin();

        // Rodzic nie reprezentuje znaku, tylko sumę częstotliwości dzieci.
        heap.insert(new HuffmanNode(left, right));
    }

    return heap.extractMin(); // korzeń drzewa Huffmana
}

/*
    Krok 3 (Huffman): generowanie kodów.
    Kod znaku = ścieżka w drzewie: lewo -> '0', prawo -> '1'.
    Dzięki temu kody są prefiksowe (można dekodować bez separatorów).
*/
static void generateCodes(HuffmanNode* node,
                          const std::string& prefix,
                          std::unordered_map<char, std::string>& codes) {
    if (!node) return;

    // Liść = konkretny znak (koniec kodu).
    if (!node->left && !node->right) {
        // Specjalny przypadek: jeśli jest tylko 1 symbol, prefix może być pusty.
        codes[node->ch] = prefix.empty() ? "0" : prefix;
        return;
    }

    generateCodes(node->left,  prefix + "0", codes);
    generateCodes(node->right, prefix + "1", codes);
}

/*
    Kompresja:
    - czytamy tekst
    - budujemy drzewo i słownik kodów
    - zamieniamy tekst na bity
    - pakujemy bity do bajtów (uint8_t)
    - zapisujemy: słownik + dane + bitCount (liczba ważnych bitów)
*/
void compressFile(const std::string& inputFile,
                  const std::string& outputFile) {

    std::string text = readTextFromFile(inputFile);

    // Pusty plik: zapisujemy "pustą paczkę" (bez słownika i bez danych).
    if (text.empty()) {
        writeCompressedFile(outputFile, {}, {}, 0);
        std::cout << "Pusty plik – zapisano pusty plik skompresowany\n";
        return;
    }

    auto freq = countFrequencies(text);
    HuffmanNode* root = buildHuffmanTree(freq);

    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    // Gdy w pliku jest jeden unikalny znak, wymuszamy kod "0".
    if (codes.size() == 1) {
        codes.begin()->second = "0";
    }

    // Pakowanie bitów do bajtów:
    std::vector<uint8_t> data;
    uint8_t currentByte = 0;  // składany bajt
    int bitPos = 0;           // ile bitów już mamy w currentByte (0..8)
    uint32_t bitCount = 0;    // ile bitów jest faktycznie zapisanych (bez paddingu)

    for (char c : text) {
        const std::string& code = codes.at(c);

        for (char b : code) {
            // Doklejamy kolejny bit do bajtu od lewej strony.
            currentByte <<= 1;
            if (b == '1') currentByte |= 1;

            bitPos++;
            bitCount++;

            // Jeśli mamy pełny bajt -> zapisujemy i zaczynamy nowy.
            if (bitPos == 8) {
                data.push_back(currentByte);
                currentByte = 0;
                bitPos = 0;
            }
        }
    }

    // Jeśli ostatni bajt jest niepełny, dopchamy zerami do 8 bitów.
    // bitCount mówi dekoderowi, ile bitów jest "prawdziwych".
    if (bitPos > 0) {
        currentByte <<= (8 - bitPos);
        data.push_back(currentByte);
    }

    writeCompressedFile(outputFile, codes, data, bitCount);

    std::cout << "OK: kompresja zakonczona\n";
}

/*
    Dekompresja:
    - wczytujemy dane, bitCount i słownik (w postaci reverseDict: "101" -> 'a')
    - czytamy bity po kolei i dokładamy do bufora
    - gdy bufor tworzy pełny kod, dopisujemy znak i czyścimy bufor
    - jeśli po końcu zostaje coś w buforze -> dane uszkodzone / ucięte
*/
void decompressFile(const std::string& inputFile,
                    const std::string& outputFile) {

    CompressedData cd = readCompressedFile(inputFile);

    std::string decoded;
    std::string buffer;

    for (uint32_t i = 0; i < cd.bitCount; ++i) {
        // Wyciągamy i-ty bit (czytamy od MSB: 7..0).
        uint8_t byte = cd.data[i / 8];
        bool bit = (byte >> (7 - (i % 8))) & 1;

        buffer += (bit ? '1' : '0');

        // Ponieważ kody są prefiksowe, gdy buffer pasuje do kodu -> mamy znak.
        auto it = cd.reverseDict.find(buffer);
        if (it != cd.reverseDict.end()) {
            decoded += it->second;
            buffer.clear();
        }
    }

    // Jeśli zostały bity, które nie tworzą żadnego kodu -> plik jest niekompletny.
    if (!buffer.empty()) {
        throw std::runtime_error("Niepelne dane – nie mozna w pelni zdekodowac pliku");
    }

    writeTextToFile(outputFile, decoded);
    std::cout << "OK: dekompresja zakonczona\n";
}

/* DEMO KOPCA (MinHeap)
   - buildFromArray (heapify)
   - insert
   - decreaseKey
   - extractMin w pętli (sortowanie rosnąco po priorytecie)
*/
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