#pragma once

#include <vector>
#include <cstdint>
#include <fstream>

constexpr size_t STATE_SIZE = 256;

void initializeKey(std::vector<uint8_t>& key, std::ifstream& keyFile) {
    size_t keyLength;
    size_t blockSize;
    keyFile >> keyLength >> blockSize;

    std::string block;
    char c;
    while(keyFile.get(c)) {
        if(c == '0' || c == '1') {
            block.push_back(c);
        }

        if(block.size() == blockSize) {
            key.push_back(std::stoi(block, nullptr, 2));
            block.clear();
        }
    }
}

void initializeState(std::vector<uint8_t>& state) {
    state.resize(STATE_SIZE);
    for(size_t i = 0; i < state.size(); i++) {
        state[i] = i;
    }
}

void parseMessage(std::vector<uint8_t>& message, std::ifstream& messageFile) {
    char c;
    while(messageFile.get(c)) {
        message.push_back(c);
    }
}

void printMessage(const std::vector<uint8_t>& message) {
    for(uint8_t c: message) {
        if(static_cast<int>(c) < 10) {
            std::cout << " ";
        }
        std::cout << static_cast<int>(c) << " ";
    }
    std::cout << std::endl;
}

void printMessageStr(const std::vector<uint8_t>& message) {
    for(uint8_t c: message) {
        std::cout << c;
    }
    std::cout << std::endl;
}

void printMessageBin(const std::vector<uint8_t>& message) {
    for(uint8_t c: message) {
        for(int i = 7; i >= 0; i--) {
            std::cout << ((c >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

void xorMessages(const std::vector<uint8_t>& message1, const std::vector<uint8_t>& message2, std::vector<uint8_t>& result) {
    result.resize(message1.size());
    for(size_t i = 0; i < message1.size(); i++) {
        result[i] = message1[i] ^ message2[i];
    }
}

void keySchedulingAlgorithm(const std::vector<uint8_t>& key, std::vector<uint8_t>& state) {
    size_t j = 0;
    for(size_t i = 0; i < state.size(); i++) {
        j = (j + state[i] + key[i % key.size()]) % state.size();
        std::swap(state[i], state[j]);
    }
}

void pseudoRandomGenerationAlgorithm(std::vector<uint8_t>& state, std::vector<uint8_t>& keyStream, size_t length) {
    keyStream.resize(length);

    size_t i = 0;
    size_t j = 0;
    for(size_t k = 0; k < length; k++) {
        i = (i + 1) % state.size();
        j = (j + state[i]) % state.size();
        std::swap(state[i], state[j]);
        keyStream[k] = state[(state[i] + state[j]) % state.size()];
    }
}

void encrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext, std::vector<uint8_t>& ciphertext) {
    std::vector<uint8_t> state;
    initializeState(state);

    keySchedulingAlgorithm(key, state);
    pseudoRandomGenerationAlgorithm(state, ciphertext, plaintext.size());

    for(size_t i = 0; i < plaintext.size(); i++) {
        ciphertext[i] ^= plaintext[i];
    }
}

void decrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& plaintext) {
    std::vector<uint8_t> state;
    initializeState(state);

    keySchedulingAlgorithm(key, state);
    pseudoRandomGenerationAlgorithm(state, plaintext, ciphertext.size());

    for(size_t i = 0; i < ciphertext.size(); i++) {
        plaintext[i] ^= ciphertext[i];
    }
}
