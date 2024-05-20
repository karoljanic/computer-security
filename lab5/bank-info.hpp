#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <string>

const std::vector<std::string> BankCodes = {
    "10101674",       // NBP Wroclaw
    "10205226",       // PKO BP SA Wroclaw
    "12406784",       // Pekao SA Wroclaw
    "10500448",       // ING Bank Slaski SA Wroclaw
    "11401140"        // mBank SA Wroclaw
};

struct BankAccount {
    std::string countryCode{"PL"};                      // 2
    std::string controlSum{"00"};                       // 2
    std::string bankCode{"00000000"};                   // 8
    std::string accountNumber{"0000000000000000"};      // 16
};

void setDigitOnPosition(BankAccount& bankAccount, size_t position, char value) {
    if(position < 2) {
        bankAccount.countryCode[position] = value;
    } else if(position < 4) {
        bankAccount.controlSum[position - 2] = value;
    } else if(position < 12) {
        bankAccount.bankCode[position - 4] = value;
    } else {
        bankAccount.accountNumber[position - 12] = value;
    }
}

int calculateControlSum(const BankAccount& bankAccount) {
    std::string rearrangedBankAccount = bankAccount.bankCode + bankAccount.accountNumber;
    for(char c: bankAccount.countryCode) {
        rearrangedBankAccount += std::to_string(c - 'A' + 10);
    }
    rearrangedBankAccount += "00";

    int64_t controlSum = 0;
    for(size_t i = 0; i < rearrangedBankAccount.size(); i++) {
        controlSum = (controlSum * 10 + (rearrangedBankAccount[i] - '0')) % 97;
    }

    return 98 - controlSum % 97;
}

bool validateBankAccount(const BankAccount& bankAccount) {
    std::string rearrangedBankAccount = bankAccount.bankCode + bankAccount.accountNumber;
    for(char c: bankAccount.countryCode) {
        rearrangedBankAccount += std::to_string(c - 'A' + 10);
    }
    rearrangedBankAccount += bankAccount.controlSum;

    int64_t controlSum = 0;
    for(size_t i = 0; i < rearrangedBankAccount.size(); i++) {
        controlSum = (controlSum * 10 + (rearrangedBankAccount[i] - '0')) % 97;
    }

    return controlSum == 1;
}

void saveBankInfo(const std::string& filename, const BankAccount& bankAccount) {
    std::ofstream file{filename};
    if(!file) {
        throw std::runtime_error("Failed to open file!");
    }

    file << bankAccount.countryCode << bankAccount.controlSum 
        << bankAccount.bankCode << bankAccount.accountNumber << std::endl;
}

BankAccount loadBankInfo(const std::string& filename) {
    std::ifstream file{filename};
    if(!file) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string bankAccountStr;
    file >> bankAccountStr;
    if(bankAccountStr.size() != 28) {
        throw std::runtime_error("Invalid bank account number!");
    }

    BankAccount bankAccount;
    bankAccount.countryCode = bankAccountStr.substr(0, 2);
    bankAccount.controlSum = bankAccountStr.substr(2, 2);
    bankAccount.bankCode = bankAccountStr.substr(4, 8);
    bankAccount.accountNumber = bankAccountStr.substr(12, 16);
    
    return bankAccount;
}

void bankAccountToVector(const BankAccount& bankAccount, std::vector<uint8_t>& message) {
    for(char c: bankAccount.countryCode) {
        message.push_back(c);
    }
    for(char c: bankAccount.controlSum) {
        message.push_back(c);
    }
    for(char c: bankAccount.bankCode) {
        message.push_back(c);
    }
    for(char c: bankAccount.accountNumber) {
        message.push_back(c);
    }
}

BankAccount vectorToBankAccount(const std::vector<uint8_t>& message) {
    BankAccount bankAccount;

    bankAccount.countryCode = std::string(message.begin(), message.begin() + 2);
    bankAccount.controlSum = std::string(message.begin() + 2, message.begin() + 4);
    bankAccount.bankCode = std::string(message.begin() + 4, message.begin() + 12);
    bankAccount.accountNumber = std::string(message.begin() + 12, message.begin() + 28);

    return bankAccount;
}