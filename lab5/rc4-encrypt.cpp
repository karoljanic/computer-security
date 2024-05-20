#include <iostream>

#include "rc4.hpp"

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <key file> <message file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream keyFile(argv[1]);
    if(!keyFile) {
        std::cerr << "Failed to open key file!" << std::endl;
        return 1;
    }

    std::ifstream messageFile(argv[2]);
    if(!messageFile) {
        std::cerr << "Failed to open message file!" << std::endl;
        return 1;
    }

    std::ofstream outputFile(argv[3]);
    if(!outputFile) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    std::vector<uint8_t> key;
    initializeKey(key, keyFile);

    std::vector<uint8_t> message;
    parseMessage(message, messageFile);

    std::vector<uint8_t> ciphertext;
    encrypt(key, message, ciphertext);
    for(size_t i = 0; i < ciphertext.size(); i++) {
        outputFile << ciphertext[i];
    }

    return 0;
}