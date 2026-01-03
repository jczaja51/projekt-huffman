#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <limits>

#include "huffman.h"

bool fileExists(const std::string& name) {
    std::ifstream f(name);
    return f.good();
}

bool endsWith(const std::string& s, const std::string& suffix) {
    if (s.size() < suffix.size()) return false;
    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string ensureExtension(const std::string& name, const std::string& ext) {
    if (endsWith(name, ext)) return name;
    return name + ext;
}

void printUsage(const char* prog) {
    std::cout
        << "Uzycie:\n"
        << "  " << prog << " compress <input.txt> <output.huf>\n"
        << "  " << prog << " decompress <input.huf> <output.txt>\n"
        << "  " << prog << " heap-demo\n";
}

void createFileWithContent(const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna utworzyc pliku");
    }

    std::cout << "Wpisz tresc pliku (pusta linia jako koniec – nacisnij ENTER dwa razy):\n";

    std::string line;
    while (true) {
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) {
            break;
        }

        file << line << '\n';
    }

    file.flush();
    file.close();
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            int choice;
            std::string in, out;

            std::cout << "\n=== Kodowanie Huffmana ===\n";
            std::cout << "1. Kompresja pliku\n";
            std::cout << "2. Dekompresja pliku\n";
            std::cout << "3. Demo kolejki priorytetowej\n";
            std::cout << "0. Wyjscie\n";
            std::cout << "Wybor: ";

            while (!(std::cin >> choice) || choice < 0 || choice > 3) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Niepoprawny wybor. Sprobuj ponownie: ";
            }

            if (choice == 0) return 0;
            if (choice == 3) {
                runHeapDemo();
                return 0;
            }

            std::cout << "Podaj plik wejsciowy: ";
            std::cin >> in;

            if (choice == 1) {
                in = ensureExtension(in, ".txt");

                if (!fileExists(in)) {
                    std::cout << "Plik \"" << in << "\" nie istnieje.\n";
                    std::cout << "Czy chcesz go teraz utworzyc? (t/n): ";

                    char ans;
                    std::cin >> ans;

                    if (ans == 't' || ans == 'T') {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        createFileWithContent(in);
                        std::cout << "Plik utworzony.\n";
                    } else {
                        throw std::runtime_error("Brak pliku wejsciowego");
                    }

                }
            } else {
                in = ensureExtension(in, ".huf");
                if (!fileExists(in)) {
                    throw std::runtime_error("Plik wejsciowy nie istnieje");
                }
            }

            std::cout << "Podaj plik wyjsciowy: ";
            std::cin >> out;

            if (choice == 1) {
                out = ensureExtension(out, ".huf");
                compressFile(in, out);
            } else {
                out = ensureExtension(out, ".txt");
                decompressFile(in, out);
            }

            return 0;
        }

        std::string mode = argv[1];

        if (mode == "heap-demo") {
            runHeapDemo();
            return 0;
        }

        if (argc != 4) {
            printUsage(argv[0]);
            return 1;
        }

        std::string in = argv[2];
        std::string out = argv[3];

        if (mode == "compress") {
            in = ensureExtension(in, ".txt");
            out = ensureExtension(out, ".huf");
            compressFile(in, out);
            return 0;
        }

        if (mode == "decompress") {
            in = ensureExtension(in, ".huf");
            out = ensureExtension(out, ".txt");
            decompressFile(in, out);
            return 0;
        }

        printUsage(argv[0]);
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << "\n";
        return 1;
    }
}