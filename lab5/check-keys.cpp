#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

void parseMessage(std::vector<uint8_t>& message, std::ifstream& messageFile) {
    char c;
    while(messageFile.get(c)) {
        message.push_back(c);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ciphertext file 1> <ciphertext file 1>" << std::endl;
        return 1;
    }

    std::ifstream ciphertextFile1(argv[1]);
    if(!ciphertextFile1) {
        std::cerr << "Failed to open ciphertext file 1!" << std::endl;
        return 1;
    }

    std::ifstream ciphertextFile2(argv[2]);
    if(!ciphertextFile2) {
        std::cerr << "Failed to open ciphertext file 2!" << std::endl;
        return 1;
    }

    std::vector<uint8_t> ciphertext1;
    parseMessage(ciphertext1, ciphertextFile1);

    std::vector<uint8_t> ciphertext2;
    parseMessage(ciphertext2, ciphertextFile2);

    // c1 = k ^ m1
    // c2 = k ^ m2
    // c1 ^ c2 = (k ^ m1) ^ (k ^ m2)
    // c1 ^ c2 = m1 ^ m2
    // ASCII has MSB = 0 => m1 ^ m2 has MSB 0 if keys are equal 

    bool keysAreEqual = true;
    for(size_t i = 0; i < ciphertext1.size(); i++) {
        if((ciphertext1[i] ^ ciphertext2[i]) > 127) {   // checking if MSB is 1
            keysAreEqual = false;
            break;
        }
    }

    if(keysAreEqual) {
        std::cout << "Keys are equal!" << std::endl;
    }
    else {
        std::cout << "Keys are not equal!" << std::endl;
    }

    return 0;
}