#include <iostream>
#include "BigInt.h"

void calc (std::string &op, std::string &arg1, std::string &arg2);

int main() {
    std::string arg1;
    std::string arg2;
    std::string op;

    while (arg1 != "stop") {
        std::cout << "To close da program write quit\n";
        std::cout << "First argument\n";

        std::cin >> arg1;
        std::cout << "Enter operator: +, -, *, /, %, >, <, ==, sqrt";
        std::cin >> op;
        if (op == "sqrt") {
            BigInt arg = BigInt(arg1).sqrt();
            std::cout << arg.to_string() << std::endl;
            continue;
        }
        std::cin >> arg2;
        calc (op, arg1, arg2);
    }

    return 0;
}

void calc (std::string &op, std::string &arg1, std::string &arg2) {
    BigInt a = BigInt(arg1);
    BigInt b = BigInt(arg2);
    if (op == "+") std::cout << (a + b).to_string() << std::endl;
    if (op == "-") std::cout << (a - b).to_string() << std::endl;
    if (op == "*") std::cout << (a * b).to_string() << std::endl;
    if (op == "/") std::cout << (a / b).to_string() << std::endl;
    if (op == "%") std::cout << (a % b).to_string() << std::endl;
    if (op == "<") std::cout << (a < b) << std::endl;
    if (op == ">") std::cout << (a > b) << std::endl;
    if (op == "==") std::cout << (a == b) << std::endl;
}