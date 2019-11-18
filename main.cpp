#include <iostream>
#include "BigInt.h"

int main() {
    std::cout << "Enter number" << std::endl;

    std::string arg1;
    std::string arg2;

    while (arg1 != "stop") {

        std::cin >> arg1;
        std::cin >> arg2;
        BigInt a = BigInt(arg1);
        BigInt b = BigInt(arg2);
        BigInt test = a / b;


        std::cout << test.to_string() << std::endl;
    }

    return 0;
}