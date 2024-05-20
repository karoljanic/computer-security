#include <iostream>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <string>
#include <random>

typedef int64_t int_t;

bool isPrime(int_t number) {
    if(number <= 1) {
        return false;
    }

    if(number <= 3) {
        return true;
    }

    if(number % 2 == 0 || number % 3 == 0) {
        return false;
    }

    int_t sqrtNumber = static_cast<int_t>(sqrt(number) + 1);
    for(int_t i = 6; i <= sqrtNumber; i += 6) {
        if(number % (i + 1) == 0 || number % (i - 1) == 0) {
            return false;
        }

    }

    return true;
}

int_t gcd(int_t num1, int_t num2) {
    int_t temp;
    while(num2 != 0) {
        temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }

    return num1;
}

int_t extendedGcd(int_t num1, int_t num2, int_t mod, int_t& x, int_t& y) {
    int_t x0 = 1, x1 = 0, y0 = 0, y1 = 1;
    int_t q, r, x2, y2;

    while(num2 > 0) {
        q = num1 / num2;
        r = num1 - q * num2;
        x2 = x0 - q * x1;
        y2 = y0 - q * y1;

        num1 = num2;
        num2 = r;
        x0 = x1;
        x1 = x2;
        y0 = y1;
        y1 = y2;
    }

    while(x0 < 0) {
        x0 += mod;
    }

    while(y0 < 0) {
        y0 += mod;
    }

    x = x0;
    y = y0;

    return num1;
}


void generate(int_t prime1, int_t prime2, int_t publicKey, const std::string& filename) {
    int_t n = prime1 * prime2;
    int_t phi = (prime1 - 1) * (prime2 - 1);

    // std::uniform_int_distribution<int_t> dist(2, phi - 1);
    // std::mt19937 gen(std::random_device{}());

    // int_t e = dist(gen);
    // while(gcd(e, phi) != 1) {
    //     e = dist(gen);
    // }

    int_t e = publicKey;

    int_t d, tmp;
    int_t g = extendedGcd(e, phi, phi, d, tmp);

    std::ofstream publicFile(filename + ".pub");
    publicFile << n << std::endl << e << std::endl;
    publicFile.close();

    std::ofstream privateFile(filename + ".priv");
    privateFile << n << std::endl << d << std::endl;
    privateFile.close();
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        std::cout << "Usage: " << argv[0] << " <prime 1> <prime 2> <pub key> <output filename>" << std::endl;
        return 1;
    }

    int_t prime1;
    try {
        prime1 = std::atoi(argv[1]);
    }
    catch(std::exception &e) {
        std::cout << "Error: <prime 1> must be integer" << std::endl;
        return 1;
    }

    int_t prime2;
    try {
        prime2 = std::atoi(argv[2]);
    }
    catch(std::exception &e) {
        std::cout << "Error: <prime 2> must be integer" << std::endl;
        return 1;
    }

    if(prime1 < 0 || prime2 < 0) {
        std::cout << "Error: <prime 1> and <prime 2> must be positive integers" << std::endl;
        return 1;
    }

    if(!isPrime(prime1) || !isPrime(prime2)) {
        std::cout << "Error: <prime 1> and <prime 2> must be prime numbers" << std::endl;
        return 1;
    }

    int_t pubKey;
    try {
        pubKey = std::atoi(argv[3]);
    }
    catch(std::exception &e) {
        std::cout << "Error: <pub key> must be integer" << std::endl;
        return 1;
    }

    generate(prime1, prime2, pubKey, argv[4]);

    return 0;
}