#include <iostream>

#include "rc4.hpp"

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <key file> <ciphertext file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream keyFile(argv[1]);
    if(!keyFile) {
        std::cerr << "Failed to open key file!" << std::endl;
        return 1;
    }

    std::ifstream ciphertextFile(argv[2]);
    if(!ciphertextFile) {
        std::cerr << "Failed to open ciphertext file!" << std::endl;
        return 1;
    }

    std::ofstream outputFile(argv[3]);
    if(!outputFile) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    std::vector<uint8_t> key;
    initializeKey(key, keyFile);

    std::vector<uint8_t> ciphertext;
    parseMessage(ciphertext, ciphertextFile);

    std::vector<uint8_t> message;
    decrypt(key, ciphertext, message);
    for(size_t i = 0; i < message.size(); i++) {
        outputFile << message[i];
    }

    return 0;
}