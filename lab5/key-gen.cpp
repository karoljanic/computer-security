#include <iostream>
#include <fstream>
#include <random>

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <key length> <block size> <output file>" << std::endl;
        return 1;
    }

    size_t keyLength;
    try {
        keyLength = std::stoul(argv[1]);
    } catch(const std::exception& e) {
        std::cerr << "Invalid key length!" << std::endl;
        return 1;
    }

    size_t blockSize;
    try {
        blockSize = std::stoul(argv[2]);
    } catch(const std::exception& e) {
        std::cerr << "Invalid block size!" << std::endl;
        return 1;
    }

    std::ofstream keyFile(argv[3]);
    if(!keyFile) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, 1);

    keyFile << keyLength << " " << blockSize << std::endl;
    for(size_t i = 0; i < keyLength; i++) {
        keyFile << std::to_string(dist(gen));
    }
    keyFile.close();
    return 0;
}