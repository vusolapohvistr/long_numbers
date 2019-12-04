#include <iostream>
#include "BigInt.h"

void calc (std::string &op, std::string &arg1, std::string &arg2);
std::vector<BigInt> stobi ();

int main() {
    std::string arg1;
    std::string arg2;
    std::string op;

    while (arg1 != "stop") {
        std::cout << "To close da program write quit\n";
        std::cout << "First argument or setmod or china\n";
        std::cin >> arg1;
        if (arg1 == "quit") break;
        if (arg1 == "china") {
            std::vector<BigInt> r = stobi();
            std::vector<BigInt> m = stobi();
            BigInt x = BigInt::chinese_tea(r, m);
            std::cout << x.to_string();
            continue;
        }
        if (arg1 == "setmod") {
            std::cout << "mod = ?\n";
            std::cin >> arg2;
            BigInt mod = BigInt(arg2);
            BigInt::set_mod(mod);
            std::cout << "mod" << (arg2[0] == '-' ? "0" : arg2) << std::endl;
            continue;
        }
        std::cout << "Enter operator: +, -, *, /, %, >, <, ==, ^, sqrt\n";
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
    if (op == "^") std::cout << (a ^ b).to_string() << std::endl;
    if (op == "<") std::cout << (a < b) << std::endl;
    if (op == ">") std::cout << (a > b) << std::endl;
    if (op == "==") std::cout << (a == b) << std::endl;
}

std::vector<BigInt> stobi () {
    std::cout << "enter args one by 1, end to end"<<std::endl;
    std::string argS = "";
    std::vector<BigInt> answer;
    do {
        std::cin >> argS;
        if (argS == "stop") break;
        answer.push_back(BigInt(argS));
    } while (true);
    return answer;
}