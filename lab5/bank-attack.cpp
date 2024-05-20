#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include <string_view>

#include "bank-info.hpp"
#include "rc4.hpp"

constexpr size_t ACCOUNT_LENGTH = 28;

const std::string ENCRYPTED_BANK_ACCOUNT_PREFIX = "data/encrypted_bank_account_";
const std::string ENCRYPTED_BANK_ACCOUNT_SUFIX = ".txt";

size_t KNOWN_DATAGRAMS = 0;

const std::vector<size_t> digitIndexes = {2, 3, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

std::map<uint8_t, std::set<uint8_t>> combinations;
std::vector<std::map<uint8_t, bool>> availableNumbers;
std::vector<std::vector<uint8_t>> filterdAvailableNumbers;
std::vector<std::vector<uint8_t>> messages;

void generateCombinations() {
    for(uint8_t i = '0'; i <= '9'; i++) {
        for(uint8_t j = '0'; j <= '9'; j++) {
            uint8_t x = i ^ j;
            if(i <= j) {
                combinations[x].insert(i);
                combinations[x].insert(j);
            }
        }
    }
}

void initializeAvailableNumbers() {
    availableNumbers.resize(ACCOUNT_LENGTH);
    for(size_t i = 0; i < ACCOUNT_LENGTH; i++) {
        availableNumbers[i] = std::map<uint8_t, bool>();
        for(uint8_t j = '0'; j <= '9'; j++) {
            availableNumbers[i][j] = true;
        }
    }
}

void readMessages(std::vector<std::vector<uint8_t>>& messages) {
    for(size_t i = 0; i < KNOWN_DATAGRAMS; i++) {
        std::ifstream messageFile{ENCRYPTED_BANK_ACCOUNT_PREFIX + std::to_string(i) + ENCRYPTED_BANK_ACCOUNT_SUFIX};
        if(!messageFile) {
            throw std::runtime_error("Failed to open file!");
        }

        parseMessage(messages[i], messageFile);
    }
}

void boundAvailableNumbers(const std::vector<std::vector<uint8_t>>& messages) {
    for(size_t i = 0; i < KNOWN_DATAGRAMS; i++) {
        std::vector<uint8_t> xoredMessage;
        xorMessages(messages[0], messages[i], xoredMessage);
        
        for(size_t j = 0; j < xoredMessage.size(); j++) {
            for(size_t k = 0; k < availableNumbers[j].size(); k++) {
                if(availableNumbers[j][k] && combinations[xoredMessage[j]].count(k) == 0) {
                    availableNumbers[j][k] = false;
                }
            }
        }
    }

    for(size_t i = 0; i < availableNumbers.size(); i++) {
        filterdAvailableNumbers.push_back(std::vector<uint8_t>());
        for(auto& [number, available]: availableNumbers[i]) {
            if(available) {
                filterdAvailableNumbers[i].push_back(number);
            }
        }
    }
}

size_t countPossibleNumbers() {
    size_t count = 1;

    for(size_t i = 2; i < 4; i++) {
        if(filterdAvailableNumbers[i].size() > 0) {
            count *= filterdAvailableNumbers[i].size();
        }
    }

    for(size_t i = 12; i < availableNumbers.size(); i++) {
        if(filterdAvailableNumbers[i].size() > 0) {
            count *= filterdAvailableNumbers[i].size();
        }
    }

    return count * BankCodes.size();
}

bool verifyKey(const std::vector<uint8_t>& key) {
    bool valid = true;

    for(size_t i = 1; i < KNOWN_DATAGRAMS; i++) {    
        std::vector<uint8_t> decryptedMessage;
        xorMessages(messages[i], key, decryptedMessage);

        if(!validateBankAccount(vectorToBankAccount(decryptedMessage))) {
            valid = false;
            break;
        }
    }

    return valid;
}

void bruteForceUtil(size_t depth, BankAccount& bankAccount, std::vector<BankAccount>& foundBankAccounts) {
    if(depth == digitIndexes.size()) {
        for(size_t i = 0; i < BankCodes.size(); i++) {
            bankAccount.bankCode = BankCodes[i];
            if(validateBankAccount(bankAccount)) {
                std::vector<uint8_t> guessMessage;
                bankAccountToVector(bankAccount, guessMessage);

                std::vector<uint8_t> key;
                xorMessages(messages[0], guessMessage, key);

                if(verifyKey(key)) {
                    foundBankAccounts.push_back(bankAccount);
                }
            }
        }
    } else {
        for(size_t i = 0; i < filterdAvailableNumbers[digitIndexes[depth]].size(); i++) {
            setDigitOnPosition(bankAccount, digitIndexes[depth], filterdAvailableNumbers[digitIndexes[depth]][i]);
            bruteForceUtil(depth + 1, bankAccount, foundBankAccounts);
        }
    }
}

BankAccount bruteForce(std::vector<BankAccount>& foundBankAccounts) {
    BankAccount bankAccount;
    bankAccount.countryCode = "PL";
    
    // initialize account number
    for(size_t i = 0; i < digitIndexes.size(); i++) {
        setDigitOnPosition(bankAccount, digitIndexes[i], filterdAvailableNumbers[digitIndexes[i]][0]);
    }
    
    bruteForceUtil(0, bankAccount, foundBankAccounts);

    return bankAccount;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number-of-known-datagrams>" << std::endl;
        return 1;
    }

    try {
        KNOWN_DATAGRAMS = std::stoi(argv[1]);
    } catch(...) {
        std::cerr << "Invalid number of known datagrams!" << std::endl;
        return 1;
    }

    generateCombinations();
    initializeAvailableNumbers();

    messages = std::vector<std::vector<uint8_t>>(KNOWN_DATAGRAMS);
    readMessages(messages); 

    boundAvailableNumbers(messages);

    std::cout << countPossibleNumbers() << std::endl;

    std::vector<BankAccount> foundBankAccounts;
    BankAccount bankAccount = bruteForce(foundBankAccounts);

    for(BankAccount& bankAccount: foundBankAccounts) {
        std::cout << bankAccount.countryCode << bankAccount.controlSum << bankAccount.bankCode << bankAccount.accountNumber << std::endl;
    }

    return 0;
}