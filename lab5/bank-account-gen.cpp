#include <iostream>
#include <random>

#include "bank-info.hpp"


int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::mt19937 gen{std::random_device{}()};

    std::uniform_int_distribution<size_t> bankCodeIndexDist{0, BankCodes.size() - 1};
    std::uniform_int_distribution<int64_t> accountNumberDist{1000000000000000, 9999999999999999};

    BankAccount bankAccount;

    bankAccount.countryCode = "PL";
    bankAccount.bankCode = BankCodes[bankCodeIndexDist(gen)];
    int64_t accountNumber = accountNumberDist(gen);
    bankAccount.accountNumber = std::to_string(accountNumber);
    bankAccount.controlSum = std::to_string(calculateControlSum(bankAccount));
    while(bankAccount.controlSum.size() < 2) {
        bankAccount.controlSum = "0" + bankAccount.controlSum;
    }

    saveBankInfo(argv[1], bankAccount);

    return 0;
}
