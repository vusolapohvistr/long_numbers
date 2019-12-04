//
// Created by news on 13.11.2019.
//
#include <string>
#include <vector>
#ifndef LONG_NUMBERS_BIGINT_H
#define LONG_NUMBERS_BIGINT_H
#define basis 100000
#define basis_length 5

class BigInt {
private:
    bool is_negative;
    std::vector<int> value; // reversed
public:
    static BigInt mod;
    explicit BigInt (std::string &number);
    explicit BigInt (int arg);
    explicit BigInt ();
    static BigInt chinese_tea(const std::vector<BigInt> &r, const std::vector<BigInt> &m);
    BigInt (std::vector<int> &number, bool is_negative);
    BigInt operator + (BigInt const &arg) const;
    BigInt operator - (BigInt const &arg) const;
    BigInt operator * (BigInt const &arg) const;
    BigInt operator / (BigInt const &arg) const;
    BigInt operator % (BigInt const &arg) const;
    BigInt operator ^ (BigInt const &arg) const;
    bool operator > (BigInt const &arg) const;
    bool operator < (BigInt const &arg) const;
    bool operator == (BigInt const &arg) const;
    std::string to_string() const;
    BigInt sqrt();
    BigInt euclid (BigInt &a, BigInt &b);
    static BigInt extended_euclid (BigInt const &b, BigInt const &m);
    static void set_mod(BigInt const &arg);
};


#endif //LONG_NUMBERS_BIGINT_H
