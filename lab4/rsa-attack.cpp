#include <iostream>
#include <fstream>
#include <cstdint>
#include <optional>

typedef int64_t int_t;

struct pq {
    int_t p;
    int_t q;
};

int_t gcd(int_t num1, int_t num2) {
    int_t temp;
    while(num2 != 0) {
        temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }

    return num1;
}

int_t powerModulo(int_t base, int_t exponent, int_t mod) {
    int_t result = 1;
    base = base % mod;

    while(exponent > 0) {
        if(exponent % 2 == 1) {
            result = (result * base) % mod;
        }

        exponent = exponent >> 1;
        base = (base * base) % mod;
    }

    return result;
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


std::optional<pq> attack(int_t n, int_t e1, int_t d1, int_t e2, int_t bound) {
    int_t kphi = d1 * e1 - 1;
    int_t t = kphi;

    while( t % 2 == 0) {
        t /= 2;
    }

    int_t a = 2, p = 1;
    while(a < bound) {
        int_t k = t;
        while(k < kphi) {
            int_t x = powerModulo(a, k, n);
            if(x != 1 && x != n - 1 && powerModulo(x, 2, n) == 1) {
                p = gcd(x - 1, n);
                break;
            }
            k *= 2;
        }

        a += 2;
    }

    if(p == 1) {
        return std::nullopt;
    }

    return pq{p, n / p};
}

int main(int argc, char** argv) {
    if(argc != 5) {
        std::cout << "Usage: " << argv[0] << " <public-key1-filename> <private-key1-filename> <public-key2-filename> <finding bound>" << std::endl;
        return 1;
    }

    std::ifstream publicFile1(argv[1]);
    std::ifstream privateFile1(argv[2]);
    std::ifstream publicFile2(argv[3]);

    int_t bound;
    try {
        bound = std::atoi(argv[4]);
    }
    catch(std::exception &e) {
        std::cout << "Error: <finding bound> must be integer" << std::endl;
        return 1;
    }

    int_t n, e1, d1, e2;
    publicFile1 >> n >> e1;
    privateFile1 >> n >> d1;
    publicFile2 >> n >> e2;

    std::optional<pq> result = attack(n, e1, d1, e2, bound);

    if(result.has_value()) {
        std::cout << "p: " << result.value().p << std::endl;
        std::cout << "q: " << result.value().q << std::endl;

        int_t phi = (result.value().p - 1) * (result.value().q - 1);
        int_t d, tmp;
        int_t g = extendedGcd(e2, phi, phi, d, tmp);

        std::cout << "private key: " << d << std::endl;
    }
    else {
        std::cout << "Attack failed" << std::endl;
    }

    return 0;
}